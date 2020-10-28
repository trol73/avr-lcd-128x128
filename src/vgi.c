#include "vgi.h"

#include "memory.h"
#include "vgi_codes.h"
#include "lcd.h"
#include "text.h"
#include "fonts.h"
#include "utils.h"

#include <stdbool.h>

extern font_config_t font_current;


uint8_t loadVgi(uint8_t size, uint8_t clearFrom, uint8_func_t loadFunc) {
//	if (clearFrom != 0xff) {
//		gui_data_size = clearFrom*4;
//	}
//	if (gui_data_size + size + 1 > GUI_DATA_SIZE) {
//		// no enough memory, skip data
//		for (uint8_t i = 0; i < size; i++) {
//			loadFunc();
//		}
//		return 0;
//	}
//	uint8_t id = gui_data_size / 4;
//	if (!id || (gui_data_size % 4)) {
//		id++;
//	}
//	uint16_t addr = id*4;
//	for (uint8_t i = 0; i < size; i++) {
//		gui_data[addr++] = loadFunc();
//	}
	uint8_t id = loadObject(size, clearFrom, loadFunc);
	if (id) {
		gui_data[gui_data_size++] = VGI_END;
	}
	//gui_data_size = addr;
	return id;
}


void drawVgi(uint8_t id, uint8_t x, uint8_t y) {
	if (!id) {
		return;
	}
	uint16_t vgiColor = 0;
	uint16_t vgiBackground;

	uint16_t addr = id*4;
	uint8_t x1, y1, x2, y2, n, n2;

	font_config_t saved_font = font_current;
	while (addr < GUI_DATA_SIZE) {
		uint8_t cmd = gui_data[addr++];
		switch (cmd) {
			case VGI_SET_COLOR:
				vgiColor = gui_data[addr++] << 8;
				vgiColor += gui_data[addr++];
				break;
			case VGI_DRAW_LINE:
				x1 = gui_data[addr++];
				y1 = gui_data[addr++];
				x2 = gui_data[addr++];
				y2 = gui_data[addr++];
				drawLine(x1 + x, y1 + y, x2 + x, y2 + y, vgiColor);
				break;
			case VGI_DRAW_POLYLINE:
				n = gui_data[addr++] - 1;
				x1 = gui_data[addr++];
				y1 = gui_data[addr++];
				while (n) {
					x2 = gui_data[addr++];
					y2 = gui_data[addr++];
					drawLine(x1 + x, y1 + y, x2 + x, y2 + y, vgiColor);
					x1 = x2;
					y1 = y2;
					n--;
				}
				break;
			case VGI_SET_FONT:
				selectFont(gui_data[addr++]);
				break;
			case VGI_DRAW_TEXT:
				x1 = gui_data[addr++];
				y1 = gui_data[addr++];

				while (true) {
					uint8_t c = gui_data[addr++];
					if (!c) {
						break;
					}
					x1 += drawCharXY(x1 + x, y1 + y, c, vgiColor);// + 1;
				}
				break;
			case VGI_DRAW_RECT:
				x1 = gui_data[addr++];
				y1 = gui_data[addr++];
				x2 = gui_data[addr++];
				y2 = gui_data[addr++];
				drawRect(x1 + x, y1 + y, x2, y2, vgiColor);
				break;
			case VGI_FILL_RECT:
				x1 = gui_data[addr++];
				y1 = gui_data[addr++];
				x2 = gui_data[addr++];
				y2 = gui_data[addr++];
				fillRect(x1 + x, y1 + y, x2, y2, vgiColor);
				break;
			case VGI_DRAW_CIRCLE:
				x1 = gui_data[addr++];
				y1 = gui_data[addr++];
				x2 = gui_data[addr++];
				drawCircle(x1 + x, y1 + y, x2, vgiColor);
				break;
			case VGI_FILL_CIRCLE:
				x1 = gui_data[addr++];
				y1 = gui_data[addr++];
				x2 = gui_data[addr++];
				fillCircle(x1 + x, y1 + y, x2, vgiColor);
				break;
			case VGI_SET_BACKGROUND:
				vgiBackground = gui_data[addr++] << 8;
				vgiBackground += gui_data[addr++];
				break;
			case VGI_DRAW_MONO_8X8:
				break;
			case VGI_FILL_MONO_8X8:
				break;
			case VGI_DRAW_GRID:
				x1 = gui_data[addr++];	// x
				y1 = gui_data[addr++];	// y
				x2 = gui_data[addr++];	// dx
				y2 = gui_data[addr++];	// dy
				n = gui_data[addr++];	// nx
				n2 = gui_data[addr++];	// ny
				for (uint8_t i = 0; i < n; i++) {
					drawVerticalLine(x1 + x + i*x2, y1 + y, (y2)*(n2-1), vgiColor);
				}
				for (uint8_t i = 0; i < n2; i++) {
					drawHorizontalLine(x1 + x, y1 + y  + i*y2, x2*(n-1)+1, vgiColor);
				}
				break;
			case VGI_DRAW_UINT8:
				x1 = gui_data[addr++];	// x
				y1 = gui_data[addr++];	// y
				n = gui_data[addr++];	// val
				drawUint16XY(x1 + x, y1 + y, n, vgiColor);
				break;
			//case VGI_END:
			default:
				font_current = saved_font;
				return;

		}
	}
}


