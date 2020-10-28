#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <unistd.h>

#include "config.h"
#include "lcd.h"
#include "text.h"
#include "keyboard.h"
#include "beeper.h"
#include "commands.h"
#include "benchmark.h"
#include "lib/uart/uart.h"

#include "simulator/display.h"
#include "fonts.h"



/*
 Цоколёвка экрана
 1. LED		подсветка (включается высоким уровнем, ~60mA)
 2. SCK		SCK сигнал шины SPI													(PB5, D13)*
 3. SDA		MOSI вывод МК															(PB3, D11)*
 4. AO		выбор data/command (если высокий уровень, то команды)							(PB2, D10)				(PB0, D8)
 5. RESET		для сброса дисплея подать высокий уровень, при работе выставить низкий			(PB1, D9)					(PB2, D10)
 6. CS		chip select, для работы с дисплеем подать низкий уровень						(PB0, D8)					(PB1, D9)
 7. GND
 8. VCC		+5В, или 3.3В если замкнуть J1 на плате экрана
 */



//#define AVR_SS_PORT			PORTB
//#define AVR_SS_PIN			2


#define TIMER1B_DELTA_COUNT	(F_CPU/256/100)		// 100 Hz


/**
 * Timers
 *
 * TC1-A: beeper frequency
 * TC1-B: 100 Hz (beeper and highlight duration)
 *
 * OC0B: buzzer
 * OC0A: led
 */

extern volatile uint8_t beeper_duration;


// 100 Hz
ISR(TIMER1_COMPB_vect) {
	OCR1B += TIMER1B_DELTA_COUNT;

	if (beeper_duration) {
		beeper_duration--;
		if (!beeper_duration) {
			BeeperStop();
		}
	}
	KeyboardCheck();
	if (screenBacklightCounter != 0) {
		if (screenBacklightCounter != 0xffff) {
			screenBacklightCounter--;
			if (screenBacklightCounter == 0) {
				BacklightDisable();
			}
		}
	}
}


volatile bool uartError;


uint8_t uartWaitNextChar() {
#if SIMULATION
	for (int i = 0; i < 100; i++) {
		int v = simulator_get_next_byte();
		if (v >= 0) {
			return v;
		}
		_delay_us(1);
	}
	return 0;
#else
	while (true) {
		uint16_t c = uart_getc();
		if (c & UART_NO_DATA) {
			continue;
		} else {
			if (c & UART_FRAME_ERROR) {
				// Framing Error detected, i.e no stop bit detected
				uart_puts_P("UART Frame Error: ");
				uartError = true;
				return 0xff;
			}
			if (c & UART_OVERRUN_ERROR) {
				// Overrun, a character already present in the UART UDR register was
				// not read by the interrupt handler before the next character arrived,
				// one or more received characters have been dropped
				uart_puts_P("UART Overrun Error: ");
				uartError = true;
				return 0xff;
			}
			if (c & UART_BUFFER_OVERFLOW) {
				// We are not reading the receive buffer fast enough,
				// one or more received character have been dropped
				uart_puts_P("Buffer overflow error: ");
				uartError = true;
				return 0xff;
			}
		}
		return (uint8_t)c;
	}
#endif
}

uint16_t uartWaitNextWord() {
	uint8_t hi = uartWaitNextChar();
	uint8_t lo = uartWaitNextChar();
	return (hi << 8) + lo;
}


int main() {
#if SIMULATION
	SimulatorInit();
#endif
	KeyboardInit();
	BeeperInit();
	KeyboardBeep(true);

	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

	// TC1
	TIMSK1 |= _BV(OCIE1B);

	sei();
	CmdInit();
	uartError = false;



	lcd_init();


#if 0
benchmark();
// восстановление после бенчмарка
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TIMSK1 = _BV(OCIE1B);
	sei();
	CmdInit();
	uartError = false;


	lcd_init();
	KeyboardInit();
	BeeperInit();
#endif

	KeyboardBeep(true);
	SetHighlightLevelHigh(DEFAULT_HIGHLIGHT_LEVEL_HIGH);
	SetHighlightLevelLow(DEFAULT_HIGHLIGHT_LEVEL_LOW);
	SetBeeperVolume(DEFAULT_BEEPER_VOLUME);

	selectFont(FONT_5x7);

	fillScreen(COLOR_BLACK);
	uint8_t x = 30;
	uint8_t y = 100;
	uint16_t color = COLOR_RED;
	x += drawCharXY(x, y, '(', color) + 1;
	x += drawCharXY(x, y, 'c', color) + 1;
	x += drawCharXY(x, y, ')', color) + 5;
	x += drawCharXY(x, y, 'T', color) + 1;
	x += drawCharXY(x, y, 'r', color) + 1;
	x += drawCharXY(x, y, 'o', color) + 1;
	x += drawCharXY(x, y, 'l', color) + 1;
	x += drawCharXY(x, y, 'S', color) + 1;
	x += drawCharXY(x, y, 'o', color) + 1;
	x += drawCharXY(x, y, 'f', color) + 1;
	x += drawCharXY(x, y, 't', color) + 1;

//	fillRect(1, 1, 100, 100, COLOR_RED);
//	fillRect(5, 5, 50, 50, COLOR_GREEN);
//	fillRect(0, 0, LCD_WIDTH, 2, COLOR_YELLOW);
//
//	setFont(Font5x7, 5, 7, 32, 0xff, STANG);
//	setFont(Tahoma13x15, 13, 15, 32, 0xff, MIKRO);


#if SIMULATION
	while (true) {
		int cmd = simulator_get_next_byte();
		if (cmd >= 0) {
			CmdExec(cmd);
			render_sdl();
		}
	}
#else
	while (true) {
		uint16_t c = uart_getc();
		//uint8_t cmd = uartWaitNextChar();
		if (!(c >> 8)) {
			drawHorizontalLine(0, 0, LCD_WIDTH, COLOR_MAGENTA);
			CmdExec(c & 0xff);
			break;
		}
		drawPixel(x, 0, OCR1B);
		if (++x >= 128) {
			x = 0;
		}
	}
	while (true) {
		uint8_t cmd = uartWaitNextChar();
		if (!uartError) {
			CmdExec(cmd);
		}
		uartError = false;
	}
#endif


//	while (1) {
//		for (uint8_t x = 0; x < 50; x++) {
//			for (uint8_t y = 0; y < 50; y++) {
//				drawPixel(5+x, 5+y, ST7735_GREEN);
//			}
//		}
//		fillRect(75, 75, 5, 2, COLOR_BLUE);
//	}
}


