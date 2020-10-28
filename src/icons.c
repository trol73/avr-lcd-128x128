#include "icons.h"
#include "lcd.h"
#include "utils.h"
#include "memory.h"


void drawBitmap8x8(uint8_t x, uint8_t y, const uint8_t* ptr, uint16_t color) {
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t dat = ptr[i];
		if (dat & _BV(0))
			drawPixel(x+7,y+i, color);
		if (dat & _BV(1))
			drawPixel(x+6,y+i, color);
		if (dat & _BV(2))
			drawPixel(x+5,y+i, color);
		if (dat & _BV(3))
			drawPixel(x+4,y+i, color);
		if (dat & _BV(4))
			drawPixel(x+3,y+i, color);
		if (dat & _BV(5))
			drawPixel(x+2,y+i, color);
		if (dat & _BV(6))
			drawPixel(x+1,y+i, color);
		if (dat & _BV(7))
			drawPixel(x+0,y+i, color);
	}
}


void fillBitmap8x8(uint8_t x, uint8_t y, const uint8_t* ptr, uint16_t color, uint16_t background) {
	drawDataStart(x, y, 8, 8);
	for (uint8_t j = 0; j < 8; j++) {
		uint8_t d = ptr[j];
		for (uint8_t i = 0; i < 8; i++) {
			if (d & _BV(7)) {
				drawDataNext(color >> 8, color);
			} else {
				drawDataNext(background >> 8, background);
			}
			d <<= 1;
		}
	}
	drawDataFinish();
}


uint8_t loadIcon(uint8_t clearFrom, uint8_func_t loadFunc) {
	return loadObject(8, clearFrom, loadFunc);
}

void drawIcon(uint8_t id, uint8_t x, uint8_t y, uint16_t color) {
	if (!id) {
		return;
	}
	uint16_t addr = id*4;
	drawBitmap8x8(x, y, &gui_data[addr], color);
}

void fillIcon(uint8_t id, uint8_t x, uint8_t y, uint16_t color, uint16_t background) {
	if (!id) {
		return;
	}
	uint16_t addr = id*4;
	fillBitmap8x8(x, y, &gui_data[addr], color, background);
}
