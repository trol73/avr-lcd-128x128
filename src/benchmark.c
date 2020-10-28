#include "benchmark.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>

#include "config.h"
#include "lcd.h"
#include "text.h"
#include "fonts.h"


#define TIMER1B_DELTA_COUNT	(F_CPU/256/100)		// 100 Hz


#define start_bm() {time_hi = 0; TCNT1 = 0; }


volatile uint16_t time_hi, time_lo;


uint32_t result_bm() {
	time_lo = TCNT1;
	uint32_t result = time_hi;
	result <<= 16;
	result += time_lo;
	return result;
}



ISR(TIMER1_OVF_vect) {
	time_hi++;
}

static void drawStringXY(const char* str, uint8_t x, uint8_t y) {
	while (true) {
		uint8_t ch = *str++;
		if (ch == 0) {
			break;
		}
		x += drawCharXY(x, y, ch, COLOR_GREEN) + 1;
	}
}

static void drawStringXYC(const char* str, uint8_t x, uint8_t y, uint16_t color) {
	while (true) {
		uint8_t ch = *str++;
		if (ch == 0) {
			break;
		}
		x += drawCharXY(x, y, ch, color) + 1;
		if (x >= 120) {
			y += 10;
			x = 0;
		}
	}
}

static void fillStringXYC(const char* str, uint8_t x, uint8_t y, uint16_t color) {
	while (true) {
		uint8_t ch = *str++;
		if (ch == 0) {
			break;
		}
		x += fillCharXY(x, y, ch, color, COLOR_BLACK) + 1;
		if (x >= 120) {
			y += 10;
			x = 0;
		}
	}
}

static void drawValue(uint8_t y, const char* name, uint32_t val) {
	drawStringXY(name, 0, y);
	
	char str[10];
	sprintf(str, "%lu", (unsigned long)val);
	drawStringXY(str, 70, y);
}


void benchmark() {
	const uint16_t colors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_CYAN, COLOR_MAGENTA, COLOR_YELLOW, COLOR_BLACK, COLOR_WHITE};
	
	selectFont(FONT_5x7);
	
	
	TCCR1A = 0;
	TCCR1B = _BV(CS12);	// divider for TC1 = 256
	TCCR1C = 0;
	TIMSK1 = _BV(TOIE1);


	uint32_t time_fill_screen = 0;
	uint32_t time_fill_circle = 0;
	uint32_t time_text_color = 0;
	uint32_t time_text_attr = 0;
	uint32_t time_text_color_big = 0;
	uint32_t time_text_attr_big = 0;
	

	// fill screen
	// 685111
	// 557102 (557106)
	start_bm();
	for (uint8_t i = 0; i < 100; i++) {
		fillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, colors[i % sizeof(colors)]);
	}
	time_fill_screen = result_bm();

	// fill circle
	// 4689494
	// 2554959
	// 2301613
	// 523016 (523568)
	start_bm();
	for (uint8_t i = 0; i < 100; i++) {
		fillCircle(64, 64, 62, colors[i % sizeof(colors)]);
	}
	time_fill_circle = result_bm();

	const char str[] = "abcdefghijklmnopqrstuvwxyz.ABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789,()[]<>";

	start_bm();
	// draw text over	
	// 742036
	// 738815
	// 640709 (643534)
	for (uint8_t i = 0; i < 100; i++) {
		drawStringXYC(str, 0, 0, colors[(i+1) % sizeof(colors)]);
		drawStringXYC(str, 0, 64, colors[(i+3) % sizeof(colors)]);
	}
	time_text_color = result_bm();
	
	// draw text with background
	// 390600 (390820)	
	start_bm();
	for (uint8_t i = 0; i < 100; i++) {
		fillStringXYC(str, 0, 0, colors[(i+1) % sizeof(colors)]);
		fillStringXYC(str, 0, 64, colors[(i+3) % sizeof(colors)]);
	}
	time_text_attr = result_bm();

	
	selectFont(FONT_TAHOMA_13x15);

	start_bm();
	// draw text over	
	// 708773
	for (uint8_t i = 0; i < 100; i++) {
		drawStringXYC(str, 0, 0, colors[(i+1) % sizeof(colors)]);
		//drawStringXYC(str, 0, 64, colors[(i+3) % sizeof(colors)]);
	}
	time_text_color_big = result_bm();
	
	// draw text with background
	// 2790363
	// 497023
	// 489723
	// 477703
	// 394549
	start_bm();
	for (uint8_t i = 0; i < 100; i++) {
		fillStringXYC(str, 0, 0, colors[(i+1) % sizeof(colors)]);
		//fillStringXYC(str, 0, 64, colors[(i+3) % sizeof(colors)]);
	}
	time_text_attr_big = result_bm();
	
	
		
	// show
	selectFont(FONT_5x7);
	fillScreen(COLOR_BLACK);
	drawValue(0, "fill.scr", time_fill_screen);
	drawValue(10, "fill.crc", time_fill_circle);
	drawValue(20, "text.clr", time_text_color);
	drawValue(30, "text.atr", time_text_attr);
	drawValue(40, "text.clr2", time_text_color_big);
	drawValue(50, "text.atr2", time_text_attr_big);
	
}