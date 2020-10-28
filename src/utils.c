#include "utils.h"

#include "lcd.h"
#include "text.h"
#include "memory.h"
#include "util/delay.h"

extern font_config_t font_current;


uint8_t drawUint16XY(uint8_t x, uint8_t y, uint16_t val, uint16_t color) {
	uint16_t num = 10000;
	bool started = false;
	uint8_t result = 0;
	while (num > 0) {
		uint8_t b = val / num;
		if (b > 0 || started || num == 1) {
			result += drawCharXY(x+result, y, '0' + b, color);

			if (font_current.width == 5 && b == 2) {
				result++;
			}

			started = true;
		}
		val -= b * num;
		num /= 10;
	}
	return result;
}


uint8_t loadObject(uint8_t size, uint8_t clearFrom, uint8_func_t loadFunc) {
	if (clearFrom != 0xff) {
		gui_data_size = clearFrom*4;
	}
	if (gui_data_size + size + 1 > GUI_DATA_SIZE) {
		// no enough memory, skip data
		for (uint8_t i = 0; i < size; i++) {
			loadFunc();
		}
		return 0;
	}
	uint8_t id = gui_data_size / 4;
	if (!id || (gui_data_size % 4 != 0)) {
		id++;
	}
	uint16_t addr = id*4;
	for (uint8_t i = 0; i < size; i++) {
		gui_data[addr++] = loadFunc();
	}
//	gui_data[addr++] = VGI_END;
	gui_data_size = addr;
	return id;
}