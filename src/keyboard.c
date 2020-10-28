#include <avr/interrupt.h>


#include "keyboard.h"

#include "beeper.h"
#include "ui.h"
#include "config.h"



uint8_t keyRepeatCount[5];

bool keyboardBeep;

// управляет подсветкой при нажатии клавиши. содержит время (в сотых долях секунды), которое будет подсвечиваться экран
// после последнего нажатия на кнопку
volatile uint16_t keyboardAutoBacklightTime = DEFAULT_HIGHLIGHT_TIME;

// счётчик обратного отсчёта подсветки экрана
volatile uint16_t screenBacklightCounter = DEFAULT_HIGHLIGHT_TIME;

volatile uint8_t highlightLevelHigh = DEFAULT_HIGHLIGHT_LEVEL_HIGH;
volatile uint8_t highlightLevelLow = DEFAULT_HIGHLIGHT_LEVEL_LOW;


static bool key_down_pressed() {
	return (PINC & _BV(2)) == 0;
}

static bool key_up_pressed() {
	return (PINC & _BV(4)) == 0;
}

static bool key_left_pressed() {
	return (PINC & _BV(1)) == 0;
}

static bool key_right_pressed() {
	return (PINC & _BV(5)) == 0;
}

static bool key_enter_pressed() {
	return (PINC & _BV(3)) == 0;
}

//static bool key_any_pressed() {
//	return (PINC & (_BV(1)|_BV(2)|_BV(3)|_BV(4)|_BV(5))) != (_BV(1)|_BV(2)|_BV(3)|_BV(4)|_BV(5));
//}


static void beepKey() {
	BeeperStart(KEYBOARD_BEEP_FREQUENCY, KEYBOARD_BEEP_DURATION);
}

void KeyboardCheck() {
	bool needHighlight = false;
	if (key_left_pressed()) {
		if (keyRepeatCount[KEY_LEFT] < 0xff) {
			keyRepeatCount[KEY_LEFT]++;
		}
		needHighlight = true;
	} else {
		keyRepeatCount[KEY_LEFT] = 0;
	}

	if (key_right_pressed()) {
		if (keyRepeatCount[KEY_RIGHT] < 0xff) {
			keyRepeatCount[KEY_RIGHT]++;
		}
		needHighlight = true;
	} else {
		keyRepeatCount[KEY_RIGHT] = 0;
	}

	if (key_up_pressed()) {
		if (keyRepeatCount[KEY_UP] < 0xff) {
			keyRepeatCount[KEY_UP]++;
		}
		needHighlight = true;
	} else {
		keyRepeatCount[KEY_UP] = 0;
	}

	if (key_down_pressed()) {
		if (keyRepeatCount[KEY_DOWN] < 0xff) {
			keyRepeatCount[KEY_DOWN]++;
		}
		needHighlight = true;
	} else {
		keyRepeatCount[KEY_DOWN] = 0;
	}

	if (key_enter_pressed()) {
		if (keyRepeatCount[KEY_ENTER] < 0xff) {
			keyRepeatCount[KEY_ENTER]++;
		}
		needHighlight = true;
	} else {
		keyRepeatCount[KEY_ENTER] = 0;
	}
	if (needHighlight) {
		screenBacklightCounter = keyboardAutoBacklightTime;
		if (screenBacklightCounter != 0) {
			BacklightEnable();
		}
	}

	bool beep = keyboardBeep;

	if (keyRepeatCount[KEY_LEFT] == KEY_PRESS_TIME) {
		onKeyPressed(KEY_LEFT);
		if (beep) {
			beepKey();
			beep = false;
		}
	}
	if (keyRepeatCount[KEY_RIGHT] == KEY_PRESS_TIME) {
		onKeyPressed(KEY_RIGHT);
		if (beep) {
			beepKey();
			beep = false;
		}
	}
	if (keyRepeatCount[KEY_UP] == KEY_PRESS_TIME) {
		onKeyPressed(KEY_UP);
		if (beep) {
			beepKey();
			beep = false;
		}
	}
	if (keyRepeatCount[KEY_DOWN] == KEY_PRESS_TIME) {
		onKeyPressed(KEY_DOWN);
		if (beep) {
			beepKey();
			beep = false;
		}
	}
	if (keyRepeatCount[KEY_ENTER] == KEY_PRESS_TIME) {
		onKeyPressed(KEY_ENTER);
		if (beep) {
			beepKey();
			beep = false;
		}
	}
}



void KeyboardInit() {
	DDRC |= ~(_BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5));
	DDR(HIGHLIGHT_PORT) |= _BV(HIGHLIGHT_PIN);

	PORTC |= _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5);

	keyRepeatCount[KEY_LEFT] = 0;
	keyRepeatCount[KEY_RIGHT] = 0;
	keyRepeatCount[KEY_UP] = 0;
	keyRepeatCount[KEY_DOWN] = 0;
	keyRepeatCount[KEY_ENTER] = 0;
	keyboardAutoBacklightTime = DEFAULT_HIGHLIGHT_TIME;
	screenBacklightCounter = keyboardAutoBacklightTime;
	if (keyboardAutoBacklightTime == 0xffff) {
		BacklightEnable();
	}
}


bool KeyboardRepeated(uint8_t key) {
	return keyRepeatCount[key] > KEY_REPEAT_TIME;
}

void KeyboardBeep(bool beep) {
	keyboardBeep = beep;
}

bool IsKeyboardBeep() {
	return keyboardBeep;
}

uint8_t KeyboardGetRepeatCount(uint8_t key) {
	return keyRepeatCount[key];
}


void BacklightDisable() {
	OCR0A = highlightLevelLow;
}

void BacklightEnable() {
	OCR0A = highlightLevelHigh;
}

void SetHighlightLevelHigh(uint8_t level) {
	highlightLevelHigh = level;
	if (screenBacklightCounter) {
		OCR0A = level;
	}
}


void SetHighlightLevelLow(uint8_t level) {
	highlightLevelLow = level;
	if (!screenBacklightCounter) {
		OCR0A = level;
	}
}