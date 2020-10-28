#include "gui_controls.h"

#include "lcd.h"
#include "gui.h"

static void _pixel(uint16_t color) {
	drawDataNext((color >> 8) & 0xff, color & 0xff);
}


void drawVerticalScrollbar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t itemIndex, uint16_t numberOfItems, uint8_t interval) {
	uint8_t items_per_screen = height / interval;
	if (numberOfItems <= items_per_screen) {
		fillRect(x, y, width, height, SCROLLBAR_BACKGROUND);
		return;
	}
	uint8_t slider_height = (uint8_t)((uint16_t)items_per_screen*height/numberOfItems);
	uint32_t dy_ = (height - slider_height) * itemIndex/ numberOfItems;
	uint8_t slider_y = y + dy_;
	if (slider_y + slider_height >= height) {
		slider_y = height - slider_height;
	}
	slider_y -= y;	
	
	drawDataStart(x, y, width, height);
	uint8_t line_x = width/2;
	for (uint8_t j = 0; j < height; j++) {
		if (j < slider_y || j > slider_y + slider_height) {
			// empty area with center dot line
			for (int i = 0; i < width; i++) {
				if (i == line_x && j % 2) {
					_pixel(SCROLLBAR_LINE_COLOR);
				} else {
					_pixel(SCROLLBAR_BACKGROUND);
				}
			}
		} else {
			if (j == slider_y) {
				for (int i = 0; i < width; i++) {
					_pixel(SCROLLBAR_SLIDER_COLOR_1);
				}
				continue;
			} else if (j == slider_y + slider_height) {
				for (int i = 0; i < width; i++) {
					_pixel(SCROLLBAR_SLIDER_COLOR_2);
				}
				continue;
			}
			// scrollbar slider
			_pixel(SCROLLBAR_SLIDER_COLOR_1);
			for (int i = 0; i < width-2; i++) {
#if !defined(SCROLLBAR_SLIDER_BACKGROUND_ALT) || SCROLLBAR_SLIDER_BACKGROUND_ALT == SCROLLBAR_SLIDER_BACKGROUND
				_pixel(SCROLLBAR_SLIDER_BACKGROUND);
#else
				_pixel((i+j) % 2 ? SCROLLBAR_SLIDER_BACKGROUND : SCROLLBAR_SLIDER_BACKGROUND_ALT);
#endif
			}
			_pixel(SCROLLBAR_SLIDER_COLOR_2);
		}
		
	}
	drawDataFinish();
}


void drawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t progress) {
//	if (gui_flags & GUI_FLAG_FIRST_PAINT) {
//		drawRect(x, y, width, height, PROGRESSBAR_FRAME_COLOR);
//		fillRect(x+1, y+1, width-2, height-2, PROGRESSBAR_FILL_BACKGROUND);
//	}
//	uint8_t segments = (width - 4 + PROGRESSBAR_SEGMENT_GAP) / (PROGRESSBAR_SEGMENT_WIDTH + PROGRESSBAR_SEGMENT_GAP);
//	uint8_t seg = (uint16_t)progress * segments / 100;
	
	drawDataStart(x, y, width, height);
	// top border line
	for (uint8_t i = 0; i < width; i++) {
		_pixel(PROGRESSBAR_FRAME_COLOR);
	}
	// frame line, background, frame line
	_pixel(PROGRESSBAR_FRAME_COLOR);
	for (uint8_t i = 0; i < width-2; i++) {
		_pixel(PROGRESSBAR_BACKGROUND);
	}
	_pixel(PROGRESSBAR_FRAME_COLOR);
	// progress segments
	uint8_t px = (uint16_t) progress*(width-4)/100;
	for (uint8_t j = 0; j < height-4; j++) {
		_pixel(PROGRESSBAR_FRAME_COLOR);
		_pixel(PROGRESSBAR_BACKGROUND);
		
		uint8_t s = 1;
		bool isSegment = true;
		uint8_t w = 0;
		for (uint8_t i = 0; i < width-4; i++) {
			w++;
			if (isSegment) {
				if (i <= px) {
					_pixel(PROGRESSBAR_FILL_BACKGROUND);
				} else {
					_pixel(PROGRESSBAR_BACKGROUND);
				}
				if (w >= PROGRESSBAR_SEGMENT_WIDTH) {
					w = 0;
					s++;
					isSegment = false;
				}
			} else {
				_pixel(PROGRESSBAR_BACKGROUND);
				if (w >= PROGRESSBAR_SEGMENT_GAP) {
					w = 0;
					isSegment = true;
				}
			}
		} // i
		_pixel(PROGRESSBAR_BACKGROUND);
		_pixel(PROGRESSBAR_FRAME_COLOR);		
	} // j

	// frame line, background, frame line
	_pixel(PROGRESSBAR_FRAME_COLOR);
	for (uint8_t i = 0; i < width-2; i++) {
		_pixel(PROGRESSBAR_BACKGROUND);
	}
	_pixel(PROGRESSBAR_FRAME_COLOR);
	
	// bottom border line
	for (uint8_t i = 0; i < width; i++) {
		_pixel(PROGRESSBAR_FRAME_COLOR);
	}	
	
	drawDataFinish();
}
