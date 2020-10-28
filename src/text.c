#include "text.h"

#include "lcd.h"


font_config_t font_current;

void drawDataStart(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void drawDataFinish();

void drawDataNext(uint8_t hi, uint8_t lo);


void setFont(PGM_P font_table, uint8_t width, uint8_t height, uint8_t start_char, uint8_t end_char, font_table_type_t type) {
	// Supports variable width fonts
	font_current.font_table = font_table;
	font_current.width = width;
	font_current.height = height;
	font_current.start_char = start_char;
	font_current.end_char = end_char;
	font_current.table_type = type; // Only supports MikroElektronika generated format at the moment
}


uint8_t drawCharXY(uint8_t x, uint8_t y, uint8_t c, uint16_t color) {
#ifdef SIMULATION
//	printf("drawCharXY(%hhu, %hhu, '%c', %hx)\n", x, y, c, color);
#endif
	if (c < font_current.start_char || c > font_current.end_char) {
		c = '.';
	}

	if (font_current.table_type == STANG) {
		// Font table in Pascal Stang format (single byte height with with no width specifier)
		// Maximum height of 8 bits only

		const char *ptr = font_current.font_table + ((c - font_current.start_char) * font_current.width);
		for (uint8_t i = 0; i < font_current.width; i++) {
		// for (uint8_t i = font_current.width-1; i >= 0; i--) {
			uint8_t dat = pgm_read_byte(ptr + i );
			if (dat & _BV(0))
				drawPixel(x+i,y+0, color);
			if (dat & _BV(1))
				drawPixel(x+i,y+1, color);
			if (dat & _BV(2))
				drawPixel(x+i,y+2, color);
			if (dat & _BV(3))
				drawPixel(x+i,y+3, color);
			if (dat & _BV(4))
				drawPixel(x+i,y+4, color);
			if (dat & _BV(5))
				drawPixel(x+i,y+5, color);
			if (dat & _BV(6))
				drawPixel(x+i,y+6, color);
// TODO !!!!!! font_height can be less than 8 !!!
			if (dat & _BV(7))
				drawPixel(x+i,y+7, color);

			//for (uint8_t j = 0; j < 8; j--) {
				// Set pixel color for each bit of the column (8-bits)
//				if (x+i >= LCD_WIDTH || y+j >= LCD_HEIGHT) {
//					// Don't try and write past the dimensions of the LCD
//					return 0;
//				}
//				if (dat & _BV(j)) {
//					drawPixel(x+i,y+j, color);
//				} else if (clearWhite) {
//					glcd_set_pixel(x+i,y+j, WHITE);
//				}
//			}
		}

		// Always return how many pixels of width were written. Here for "stang" format fonts, it is always fixed
		return font_current.width;

	} else if (font_current.table_type == MIKRO) {
		/* Font table in MikroElecktronica format
		   - multi row fonts allowed (more than 8 pixels high)
		   - variable width fonts allowed
		   a complete column is written before moving to the next */

		uint8_t var_width;

		uint8_t bytes_high = font_current.height / 8 + 1;
		uint8_t bytes_per_char = font_current.width * bytes_high + 1; /* The +1 is the width byte at the start */

		const char *p = font_current.font_table + (c - font_current.start_char) * bytes_per_char;

		/* The first byte per character is always the width of the character */
		var_width = pgm_read_byte(p);
		p++; /* Step over the variable width field */

		/*
		if (x+var_width >= GLCD_LCD_WIDTH || y+font_current.height >= GLCD_LCD_HEIGHT) {
			return;
		}
		*/

		for (uint8_t i = 0; i < var_width; i++) {
			for (uint8_t j = 0; j < bytes_high; j++) {
				uint8_t dat = pgm_read_byte( p + i*bytes_high + j);
				for (uint8_t bit = 0; bit < 8; bit++) {

					if (x+i >= LCD_WIDTH || y+j*8+bit >= LCD_HEIGHT) {
						// Don't write past the dimensions of the LCD, skip the entire char
						return 0;
					}

					// We should not write if the y bit exceeds font height
					if ((j*8 + bit) >= font_current.height) {
						// Skip the bit
						continue;
					}

					if (dat & (1<<bit)) {
						drawPixel(x+i, y+j*8+bit, color);
//					} else {
//						glcd_set_pixel(x+i,y+j*8+bit,WHITE);
					}
				}
			}
		}
		return var_width;

	} else if (font_current.table_type == GLCD_UTILS) {
		/* Font table format of glcd-utils
		   - A complete row is written first (not completed columns)
		   - Width not stored, but we can search and determine it
		   - Not yet supported */

		uint8_t bytes_high = font_current.height / 8 + 1;
		uint8_t bytes_per_char = font_current.width * bytes_high;

		// Point to chars first byte
		const char *p = font_current.font_table + (c - font_current.start_char) * bytes_per_char;

		// Determine the width of the character
		uint8_t var_width = font_current.width;

		uint8_t n = 0; // How many columns back from the end

		while (1) {
			uint8_t max_byte = 0;
			uint8_t row = 0;

			for (row = 0; row < bytes_high; row++) {
				uint8_t offset;
				offset = (font_current.width - 1 - n) * row;
				max_byte = *(p + offset);
			}
			if (max_byte == 0) {
				// column is empty for all rows, go left and test again
				// reduce variable width by 1
				var_width--;
				if (var_width == 0) {
					break;
				}
			} else {
				break; /* Part of a character was found */
			}
			n++;
		}

		// Uncomment line below, to force fixed width, for debugging only
		//var_width = font_current.width; // bypass auto width detection, treat as fixed width

		// For glcd-utils format, we write one complete row at a time
		for (uint8_t j = 0; j < bytes_high; j++ ) {	// loop as rows, 1st row, j=0
			// Loop one row at a time

			for (uint8_t i = 0; i < var_width; i++ ) {
				// Loop one column at a time

				uint8_t dat = pgm_read_byte( p + j*font_current.width + i );
				for (uint8_t bit = 0; bit < 8; bit++) {

					if ((x+i) >= LCD_WIDTH || (y+j*8+bit) >= LCD_HEIGHT) {
						// Don't write past the dimensions of the LCD, skip the entire char
						return 0;
					}

					// We should not write if the y bit exceeds font height
					if ((j*8 + bit) >= font_current.height) {
						// Skip the bit
						continue;
					}

					if (dat & (1<<bit)) {
						drawPixel(x+i, y+j*8+bit, color);
//					} else {
//						drawPixel(x+i, y+j*8+bit, WHITE);
					}
				}
			} // i
		} // j

		return var_width; /* Number of columns written to display */

	} else {
		// Don't recognise the font table
		return 0;

	}

}


uint8_t fillCharXY(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t background) {
//#ifdef SIMULATION
//	printf("fillCharXY(%hhu, %hhu, '%c', %hx, %hx)\n", x, y, c, color, background);
//#endif
	if (c < font_current.start_char || c > font_current.end_char) {
#ifdef SIMULATION
	printf("wrong char '%c' (%hhu), available area is[%hhu .. %hhu]\n", c, c, font_current.start_char, font_current.end_char);
#endif
		c = '.';
	}
	if (font_current.table_type == STANG) {	// TODO !!!! use handler here !!!!!!!!!!!!!!!!
		// Font table in Pascal Stang format (single byte height with with no width specifier)
		// Maximum height of 8 bits only

		drawDataStart(x, y, font_current.width, font_current.height);

		for (uint8_t j = 0; j < font_current.height; j++) {
			for (uint8_t i = 0; i < font_current.width; i++) {
				uint8_t dat = pgm_read_byte(font_current.font_table + ((c - font_current.start_char) * (font_current.width)) + i);

				// Set pixel color for each bit of the column (8-bits)
				if (x+i >= LCD_WIDTH || y+j >= LCD_HEIGHT) {
					// Don't try and write past the dimensions of the LCD
					return 0;
				}

				if (dat & _BV(j)) {
					drawDataNext(color >> 8, color);
				} else {
					drawDataNext(background >> 8, background);
				}
			}
		}
		drawDataFinish();

		// Always return how many pixels of width were written. Here for "stang" format fonts, it is always fixed
		return font_current.width;

	} else if (font_current.table_type == MIKRO) {
		/* Font table in MikroElecktronica format
		   - multi row fonts allowed (more than 8 pixels high)
		   - variable width fonts allowed
		   a complete column is written before moving to the next */

		uint8_t var_width;

		uint8_t bytes_high = font_current.height / 8 + 1;
		uint8_t bytes_per_char = font_current.width * bytes_high + 1; // The +1 is the width byte at the start

		const char *p = font_current.font_table + (c - font_current.start_char) * bytes_per_char;

		// The first byte per character is always the width of the character
		var_width = pgm_read_byte(p);
		p++; /* Step over the variable width field */

		/*
		if (x+var_width >= GLCD_LCD_WIDTH || y+font_current.height >= GLCD_LCD_HEIGHT) {
			return;
		}
		*/
		if (x+var_width >= LCD_WIDTH || y+bytes_high*8 >= LCD_HEIGHT) {
			// Don't write past the dimensions of the LCD, skip the entire char
			return 0;
		}

		drawDataStart(x, y, var_width, bytes_high*8);

		for (uint8_t j = 0; j < bytes_high; j++) {
			for (uint8_t bit = 0; bit < 8; bit++) {
				// We should not write if the y bit exceeds font height
				if ((j*8 + bit) >= font_current.height) {
					// Skip the bit
					break;
				}

				for (uint8_t i = 0; i < var_width; i++) {
					uint8_t dat = pgm_read_byte( p + i*bytes_high + j);


//					if (dat & _BV(bit)) {
//						drawPixel(x+i, y+j*8+bit, color);
//					} else {
//						drawPixel(x+i, y+j*8+bit, background);
//					}
					if (dat & _BV(bit)) {
						drawDataNext(color >> 8, color);
					} else {
						drawDataNext(background >> 8, background);
					}

				}
			}
		}
		drawDataFinish();
		return var_width;

	} else if (font_current.table_type == GLCD_UTILS) {
		/* Font table format of glcd-utils
		   - A complete row is written first (not completed columns)
		   - Width not stored, but we can search and determine it
		   - Not yet supported */

		uint8_t bytes_high = font_current.height / 8 + 1;
		uint8_t bytes_per_char = font_current.width * bytes_high;

		// Point to chars first byte
		const char *p = font_current.font_table + (c - font_current.start_char) * bytes_per_char;

		// Determine the width of the character
		uint8_t var_width = font_current.width;

		uint8_t n = 0; // How many columns back from the end

		while (1) {
			uint8_t max_byte = 0;
			uint8_t row = 0;

			for (row = 0; row < bytes_high; row++) {
				uint8_t offset;
				offset = (font_current.width - 1 - n) * row;
				max_byte = *(p + offset);
			}
			if (max_byte == 0) {
				// column is empty for all rows, go left and test again
				// reduce variable width by 1
				var_width--;
				if (var_width == 0) {
					break;
				}
			} else {
				break; /* Part of a character was found */
			}
			n++;
		}

		// Uncomment line below, to force fixed width, for debugging only
		//var_width = font_current.width; // bypass auto width detection, treat as fixed width

		// For glcd-utils format, we write one complete row at a time
		for (uint8_t j = 0; j < bytes_high; j++ ) {	// loop as rows, 1st row, j=0
			// Loop one row at a time

			for (uint8_t i = 0; i < var_width; i++ ) {
				// Loop one column at a time

				uint8_t dat = pgm_read_byte( p + j*font_current.width + i );
				for (uint8_t bit = 0; bit < 8; bit++) {

					if ((x+i) >= LCD_WIDTH || (y+j*8+bit) >= LCD_HEIGHT) {
						// Don't write past the dimensions of the LCD, skip the entire char
						return 0;
					}

					// We should not write if the y bit exceeds font height
					if ((j*8 + bit) >= font_current.height) {
						// Skip the bit
						continue;
					}

					if (dat & (1<<bit)) {
						drawPixel(x+i, y+j*8+bit, color);
//					} else {
//						drawPixel(x+i, y+j*8+bit, WHITE);
					}
				}
			} // i
		} // j

		return var_width; /* Number of columns written to display */

	} else {
		// Don't recognise the font table
		return 0;

	}

}

uint8_t getCharWidth(uint8_t ch) {
	if (ch < font_current.start_char || ch > font_current.end_char) {
		return 0;
	}

	if (font_current.table_type == STANG) {
		return font_current.width;
	} else if (font_current.table_type == MIKRO) {
		uint8_t bytes_high = font_current.height / 8 + 1;
		uint8_t bytes_per_char = font_current.width * bytes_high + 1; // The +1 is the width byte at the start
		const char *p = font_current.font_table + (ch - font_current.start_char) * bytes_per_char;

		// The first byte per character is always the width of the character
		return pgm_read_byte(p);
	}
	return 0;
}



