#include "commands.h"

#include <avr/boot.h>

#include "api_codes.h"

#include "lib/uart/uart.h"
#include "lcd.h"
#include "text.h"
#include "gui_controls.h"

#include "beeper.h"
#include "keyboard.h"
#include "fonts.h"
#include "vgi.h"
#include "icons.h"

#if SIMULATION
extern uint32_t write_after_sync_cnt;
#endif


void_proc_t cmd_handlers[API_MAX_CODE+1];

#define MAX_STR_ARG_SIZE		0xff

extern volatile bool uartError;

uint8_t uartWaitNextChar();
uint16_t uartWaitNextWord();

uint8_t cmd_x, cmd_y;
uint8_t cmd_string_interval_x = 0, cmd_string_interval_y = 0;
uint16_t cmd_color;
uint16_t cmd_background;
uint8_t cmd_fill_line_x1 = 0, cmd_fill_line_x2 = LCD_WIDTH-1;

static void drawStringAnchor(uint8_t x, uint8_t y, const char* str, uint8_t anchor);
static void fillStringAnchor(uint8_t x, uint8_t y, const char* str, uint8_t anchor);

static inline void write_byte(uint8_t b) {
#if SIMULATION
	server_write_byte(b);
#else
	uart_putc(b);
#endif
}

static void write_str(const char *s){
    while (*s) {
      write_byte(*s++);
	 }
}

static void uart_putc_hex(uint8_t b) {
	// upper nibble
	if ((b >> 4) < 0x0a) {
		write_byte((b >> 4) + '0');
	} else {
		write_byte((b >> 4) - 0x0a + 'a');
	}

	// lower nibble
	if ((b & 0x0f) < 0x0a) {
		write_byte((b & 0x0f) + '0');
	} else {
		write_byte((b & 0x0f) - 0x0a + 'a');
	}
}



static void cmd_sync() {
	uint8_t c = uartWaitNextChar();
	if (!uartError) {
		write_byte(c);
	}
#if SIMULATION
	write_after_sync_cnt = 0;
#endif
	uartError = false;
}

static void cmd_getVersion() {
	write_byte(1);
}

static void cmd_startBootloader() {
#ifndef SIMULATION
	uint8_t hi_fuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
	switch (hi_fuse & 6) {
		case 0:
			asm("ijmp" :: "z" (0x3800)); // size = 2048 words
		case 2:
			asm("ijmp" :: "z" (0x3C00)); // size = 1024 words
		case 4:
			asm("ijmp" :: "z" (0x3E00)); // size = 512 words
		case 6:
			asm("ijmp" :: "z" (0x3F00));	// size = 256 words
	}
#endif
	BeeperStart(1000, 150);
}



static void cmd_checkKeyboard() {
	uint8_t mask = 0;
	uint8_t codes[5];
	for (uint8_t key = 0; key < 5; key++) {
		uint8_t cnt = KeyboardGetRepeatCount(key);
		if (cnt) {
			mask |= _BV(key);
		}
		codes[key] = cnt;
	}
	write_byte(mask);
	if (mask) {
		for (uint8_t key = 0; key < 5; key++) {
			uint8_t cnt = codes[key];
			if (cnt) {
				write_byte(cnt);
			}
		}
	}
}

static void cmd_setBacklightTime() {
	uint16_t time = uartWaitNextWord();
	if (!uartError) {
		keyboardAutoBacklightTime = time;
		if (time == 0) {
			BacklightDisable();
		} else if (time == 0xffff) {
			BacklightEnable();
		} else {
			if (screenBacklightCounter == 0xffff || screenBacklightCounter == 0) {
				screenBacklightCounter = time;
			}
		}
	}
}

static void cmd_setStringIntervalX() {
	uint8_t dx = uartWaitNextChar();
	if (!uartError) {
		cmd_string_interval_x = dx;
	}
}

static void cmd_setStringIntervalY() {
	uint8_t dy = uartWaitNextChar();
	if (!uartError) {
		cmd_string_interval_y = dy;
	}
}

static void cmd_setKeyboardBeep() {
	uint8_t enable = uartWaitNextChar();
	if (!uartError) {
		KeyboardBeep(enable);
	}
}

static void cmd_setHightlightLevelHigh() {
	uint8_t level = uartWaitNextChar();
	if (!uartError) {
		SetHighlightLevelHigh(level);
	}
}

static void cmd_setHightlightLevelLow() {
	uint8_t level = uartWaitNextChar();
	if (!uartError) {
		SetHighlightLevelLow(level);
	}
}


static void cmd_beep() {
	uint16_t f = uartWaitNextWord();
	uint8_t d = uartWaitNextChar();
	if (!uartError) {
		BeeperStart(f, d);
	}
}

static void cmd_playMelody() {
	// TODO implement me !!!
}


static void cmd_setBeeperVolume() {
	uint8_t volume = uartWaitNextChar();
	if (!uartError) {
		SetBeeperVolume(volume);
	}
}

static void cmd_lcdFillScreen() {
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		fillScreen(color);
//		cmd_string_interval_x = 1;
//		cmd_string_interval_y = 1;
		cmd_x = 0;
		cmd_y = 0;
	}
}

static void cmd_setColor() {
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		cmd_color = color;
	}
}

static void cmd_setBackground() {
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		cmd_background = color;
	}
}

static void cmd_setFont() {
	uint8_t font = uartWaitNextChar();
	if (!uartError) {
		selectFont(font);
	}
}


static void cmd_fillRect() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t w = uartWaitNextChar();
	uint8_t h = uartWaitNextChar();
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		fillRect(x, y, w, h, color);
	}
}


static void cmd_fillRectDefault() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t w = uartWaitNextChar();
	uint8_t h = uartWaitNextChar();
	if (!uartError) {
		fillRect(x, y, w, h, cmd_background);
	}
}

static void cmd_drawPixel() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	if (!uartError) {
		drawPixel(x, y, cmd_color);
	}
}

static void cmd_drawLine() {
	uint8_t x0 = uartWaitNextChar();
	uint8_t y0 = uartWaitNextChar();
	uint8_t x1 = uartWaitNextChar();
	uint8_t y1 = uartWaitNextChar();
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		drawLine(x0, y0, x1, y1, color);
		cmd_x = x1;
		cmd_y = y1;
	}
}

static void cmd_drawLineDefault() {
	uint8_t x0 = uartWaitNextChar();
	uint8_t y0 = uartWaitNextChar();
	uint8_t x1 = uartWaitNextChar();
	uint8_t y1 = uartWaitNextChar();
	if (!uartError) {
		drawLine(x0, y0, x1, y1, cmd_color);
		cmd_x = x1;
		cmd_y = y1;
	}
}

static void cmd_drawLineTo() {
	uint8_t x1 = uartWaitNextChar();
	uint8_t y1 = uartWaitNextChar();
	if (!uartError) {
		drawLine(cmd_x, cmd_y, x1, y1, cmd_color);
		cmd_x = x1;
		cmd_y = y1;
	}
}

static void cmd_drawRect() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t w = uartWaitNextChar();
	uint8_t h = uartWaitNextChar();
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		drawRect(x, y, w, h, color);
	}
}

static void cmd_drawRectDefault() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t w = uartWaitNextChar();
	uint8_t h = uartWaitNextChar();
	if (!uartError) {
		drawRect(x, y, w, h, cmd_color);
	}
}

static void cmd_drawRectThick() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t w = uartWaitNextChar();
	uint8_t h = uartWaitNextChar();
	uint8_t tx = uartWaitNextChar();
	uint8_t ty = uartWaitNextChar();
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		drawRectThick(x, y, w, h, tx, ty, color);
	}
}

static void cmd_drawRectThickDefault() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t w = uartWaitNextChar();
	uint8_t h = uartWaitNextChar();
	uint8_t tx = uartWaitNextChar();
	uint8_t ty = uartWaitNextChar();
	if (!uartError) {
		drawRectThick(x, y, w, h, tx, ty, cmd_color);
	}
}

static void cmd_drawCircle() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t r = uartWaitNextChar();
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		drawCircle(x, y, r, color);
	}
}

static void cmd_drawCircleDefault() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t r = uartWaitNextChar();
	if (!uartError) {
		drawCircle(x, y, r, cmd_color);
	}
}

static void cmd_fillCircle() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t r = uartWaitNextChar();
	uint16_t color = uartWaitNextWord();
	if (!uartError) {
		fillCircle(x, y, r, color);
	}
}

static void cmd_fillCircleDefault() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t r = uartWaitNextChar();
	if (!uartError) {
		fillCircle(x, y, r, cmd_background);
	}
}

static void cmd_drawCharXY() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t ch = uartWaitNextChar();
	if (!uartError) {
		cmd_x = x + drawCharXY(x, y, ch, cmd_color);
		cmd_y = y;
	}
}

static void cmd_drawChar() {
	uint8_t ch = uartWaitNextChar();
	if (!uartError) {
		cmd_x += drawCharXY(cmd_x, cmd_y, ch, cmd_color) + cmd_string_interval_x;
	}
}

static void cmd_drawString() {
	while (true) {
		uint8_t ch = uartWaitNextChar();
		if (uartError) {
			return;
		}
		if (ch == 0) {
			break;
		}
		cmd_x += drawCharXY(cmd_x, cmd_y, ch, cmd_color) + cmd_string_interval_x;
		if (cmd_x + font_current.width > LCD_WIDTH) {
			cmd_y += font_current.height + cmd_string_interval_y;
			cmd_x = 0;
		}
	}
}

static void cmd_fillChar() {
	uint8_t ch = uartWaitNextChar();
	if (!uartError) {
		cmd_x += fillCharXY(cmd_x, cmd_y, ch, cmd_color, cmd_background);
		if (cmd_x < LCD_WIDTH-1) {
			fillRect(cmd_x, cmd_y, cmd_string_interval_x, font_current.height+cmd_string_interval_y, cmd_background);
			cmd_x += cmd_string_interval_x;
		}
	}
}

static void cmd_fillCharXY() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t ch = uartWaitNextChar();
	if (!uartError) {
		cmd_x = x + fillCharXY(x, y, ch, cmd_color, cmd_background);
		cmd_y = y;
		if (cmd_x < LCD_WIDTH-1) {
			fillRect(cmd_x, cmd_y, cmd_string_interval_x, font_current.height+cmd_string_interval_y, cmd_background);
			cmd_x += cmd_string_interval_x;
		}
	}
}

static void cmd_fillString() {
	while (true) {
		uint8_t ch = uartWaitNextChar();
		if (uartError) {
			return;
		}
		if (ch == 0) {
			break;
		}

		cmd_x += fillCharXY(cmd_x, cmd_y, ch, cmd_color, cmd_background) + cmd_string_interval_x;
		if (cmd_x < LCD_WIDTH-1) {
			fillRect(cmd_x, cmd_y, cmd_string_interval_x, font_current.height+cmd_string_interval_y, cmd_background);
		}
		if (cmd_x + font_current.width > LCD_WIDTH) {
			cmd_y += font_current.height + cmd_string_interval_y;
			cmd_x = 0;
		}
	}
}


static void cmd_drawStringAnchor() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t anchor = uartWaitNextChar();
	if (uartError) {
		return;
	}

	char str[MAX_STR_ARG_SIZE];
	str[sizeof(str)-1] = 0;
	uint8_t i = 0;
	for (;;) {
		char ch = uartWaitNextChar();
		if (uartError) {
			return;
		}
		if (i < sizeof(str)) {
			str[i++] = ch;
		}
		if (!ch) {
			break;
		}
	}
	drawStringAnchor(x, y, str, anchor);
}

static void cmd_fillStringAnchor() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t anchor = uartWaitNextChar();
	if (uartError) {
		return;
	}

	char str[MAX_STR_ARG_SIZE];
	str[sizeof(str)-1] = 0;
	uint8_t i = 0;
	for (;;) {
		char ch = uartWaitNextChar();
		if (uartError) {
			return;
		}
		if (i < sizeof(str)) {
			str[i++] = ch;
		}
		if (!ch) {
			break;
		}
	}
	fillStringAnchor(x, y, str, anchor);
}

static void cmd_setFillStringRegion() {
	uint8_t x1 = uartWaitNextChar();
	uint8_t x2 = uartWaitNextChar();
	if (!uartError) {
		cmd_fill_line_x1 = x1;
		cmd_fill_line_x2 = x2;
	}
}


static void cmd_drawProgressBar() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t w = uartWaitNextChar();
	uint8_t h = uartWaitNextChar();
	uint8_t progress = uartWaitNextChar();
	if (!uartError) {
		drawProgressBar(x, y, w, h, progress);
	}
}

static void cmd_drawVerticalScrollbar() {
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	uint8_t w = uartWaitNextChar();
	uint8_t h = uartWaitNextChar();
	uint16_t index = uartWaitNextWord();
	uint16_t items = uartWaitNextWord();
	uint8_t interval = uartWaitNextChar();

	if (!uartError) {
		drawVerticalScrollbar(x, y, w, h, index, items, interval);
	}
}
static void cmd_loadVgi() {
	uint8_t size = uartWaitNextChar();
	uint8_t clearFrom = uartWaitNextChar();
	if (!uartError) {
		uint8_t id = loadVgi(size, clearFrom, &uartWaitNextChar);
		write_byte(id);
	}
}

static void cmd_drawVgi() {
	uint8_t id = uartWaitNextChar();
	if (!uartError) {
		drawVgi(id, 0, 0);
	}
}

static void cmd_drawVgiXY() {
	uint8_t id = uartWaitNextChar();
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	if (!uartError) {
		drawVgi(id, x, y);
	}
}

static void cmd_loadIcon() {
	uint8_t clearFrom = uartWaitNextChar();
	if (!uartError) {
		uint8_t id = loadIcon(clearFrom, &uartWaitNextChar);
		write_byte(id);
	}
}

static void cmd_drawIcon() {
	uint8_t id = uartWaitNextChar();
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	if (!uartError) {
		drawIcon(id, x, y, cmd_color);
	}
}

static void cmd_fillIcon() {
	uint8_t id = uartWaitNextChar();
	uint8_t x = uartWaitNextChar();
	uint8_t y = uartWaitNextChar();
	if (!uartError) {
		fillIcon(id, x, y, cmd_color, cmd_background);
	}
}



//void uart_putc_hex(uint8_t b);
extern volatile uint8_t highlightLevel;

void cmd_debug() {
	write_str("fuse_lo=");
	uint8_t f = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
	uart_putc_hex(f);
	write_byte('\n');
	write_str("fuse_hi=");
	f = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
	uart_putc_hex(f);
	write_byte('\n');
	write_str("fuse_ex=");
	f = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
	uart_putc_hex(f);
	write_byte('\n');

	write_str("DDRD=");
	uart_putc_hex(DDRD);
	write_byte('\n');
	write_str("PORTD=");
	uart_putc_hex(PORTD);
	write_byte('\n');
	write_str("PIND=");
	uart_putc_hex(PIND);
	write_byte('\n');

	write_str("OCR0A=");
	uart_putc_hex(OCR0A);
	write_byte('\n');

	write_str("OCR0B=");
	uart_putc_hex(OCR0B);
	write_byte('\n');


}


static void drawStringAnchor(uint8_t x, uint8_t y, const char* str, uint8_t anchor) {
	uint8_t width = 0;

	if (anchor & (ANCHOR_CENTER_H | ANCHOR_RIGHT)) {
		const char *strc = str;
		for (;;) {
			char ch = *strc++;
			if (!ch) {
				break;
			} else {
				width += getCharWidth(ch) + cmd_string_interval_x;
			}
		}
	}

	if (anchor & ANCHOR_CENTER_H) {
		x -= width/2;
	} else if (anchor & ANCHOR_RIGHT) {
		x -= width;
	}
	if (anchor & ANCHOR_CENTER_V) {
		y -= font_current.height/2;
	} else if (anchor & ANCHOR_BOTTOM) {
		y -= font_current.height;
	}

	while (true) {
		char ch = *str++;

		if (!ch) {
			break;
		}
		if (x + getCharWidth(ch) > LCD_WIDTH) {
			if (anchor & ANCHOR_AUTOWRAP) {
				x = 0;
				y += font_current.height + cmd_string_interval_y;
				if (y >= LCD_HEIGHT) {
					break;
				}
			} else {
				break;
			}
		}
		x += drawCharXY(x, y, ch, cmd_color) + cmd_string_interval_x;
//		if (x >=	LCD_WIDTH) {
//
//		}
	}
	cmd_x = x;
	cmd_y = y;
}

static void fillStringAnchor(uint8_t x, uint8_t y, const char *str, uint8_t anchor) {
	uint8_t width = 0;

	if (anchor & (ANCHOR_CENTER_H | ANCHOR_RIGHT)) {
		const char *strc = str;
		for (;;) {
			char ch = *strc++;
			if (!ch) {
				break;
			} else {
				width += getCharWidth(ch) + cmd_string_interval_x;
			}
		}
	}

	if (anchor & ANCHOR_CENTER_H) {
		x -= width/2;
	} else if (anchor & ANCHOR_RIGHT) {
		x -= width;
	}
	if (anchor & ANCHOR_CENTER_V) {
		y -= font_current.height/2;
	} else if (anchor & ANCHOR_BOTTOM) {
		y -= font_current.height;
	}

	if (anchor & ANCHOR_FULL_LINE) {
		//заливка слева от строки
		fillRect(cmd_fill_line_x1, y, x, font_current.height + cmd_string_interval_y, cmd_background);
	}

	while (true) {
		char ch = *str++;

		if (!ch) {
			break;
		}
		if (x + getCharWidth(ch) > LCD_WIDTH) {
			if (anchor & ANCHOR_AUTOWRAP) {
				x = 0;
				y += font_current.height + cmd_string_interval_y;
				if (y >= LCD_HEIGHT) {
					break;
				}
			} else {
				break;
			}
		}
		uint8_t dx = fillCharXY(x, y, ch, cmd_color, cmd_background);
		if (cmd_string_interval_y) {
			fillRect(x, y + font_current.height, dx, cmd_string_interval_y, cmd_background);
		}
		x += dx;
		fillRect(x, y, cmd_string_interval_x, font_current.height + cmd_string_interval_y, cmd_background);
		x += cmd_string_interval_x;
		if (x >=	LCD_WIDTH) {
//			if (anchor & ANCHOR_AUTOWRAP) {
//				x = 0;
//				y += font_current.height + cmd_string_interval_y;
//				if (y >= LCD_HEIGHT) {
//					break;
//				}
//			} else {
//				break;
//			}
		}
	}
	if (anchor & ANCHOR_FULL_LINE) {
		// заливка справа от строки
		fillRect(x, y, cmd_fill_line_x2-x+1, font_current.height + cmd_string_interval_y, cmd_background);
	}
	cmd_x = x;
	cmd_y = y;
}


void CmdInit() {
	cmd_handlers[CMD_GET_VERSION] = &cmd_getVersion;	
	cmd_handlers[CMD_SYNC] = &cmd_sync;
	cmd_handlers[CMD_START_BOOTLOADER] = &cmd_startBootloader;
	cmd_handlers[CMD_SET_BACKLIGHT_TIME] = &cmd_setBacklightTime;
	cmd_handlers[CMD_SET_BACKLIGHT_LEVEL_LOW] = &cmd_setHightlightLevelLow;
	cmd_handlers[CMD_SET_BACKLIGHT_LEVEL_HIGH] = &cmd_setHightlightLevelHigh;
	cmd_handlers[CMD_SET_BEEPER_VOLUME] = &cmd_setBeeperVolume;
	cmd_handlers[CMD_SET_KEYBOARD_BEEP] = &cmd_setKeyboardBeep;
	cmd_handlers[CMD_READ_KEYBOARD] = &cmd_checkKeyboard;
	cmd_handlers[CMD_BEEP] = &cmd_beep;
	cmd_handlers[CMD_PLAY_MELODY] = &cmd_playMelody;
	cmd_handlers[CMD_SET_STRING_INTERVAL_X] = &cmd_setStringIntervalX;
	cmd_handlers[CMD_SET_STRING_INTERVAL_Y] = &cmd_setStringIntervalY;
	cmd_handlers[CMD_SET_COLOR] = &cmd_setColor;
	cmd_handlers[CMD_SET_BACKGROUND] = &cmd_setBackground;
	cmd_handlers[CMD_SET_FONT] = &cmd_setFont;
	cmd_handlers[CMD_DRAW_PIXEL] = &cmd_drawPixel;
	cmd_handlers[CMD_DRAW_LINE_DEFAULT] = &cmd_drawLineDefault;
	cmd_handlers[CMD_FILL_SCREEN] = &cmd_lcdFillScreen;
	cmd_handlers[CMD_DRAW_LINE] = &cmd_drawLine;
	cmd_handlers[CMD_DRAW_LINE_TO] = &cmd_drawLineTo;
	cmd_handlers[CMD_DRAW_RECT] = &cmd_drawRect;
	cmd_handlers[CMD_DRAW_RECT_DEFAULT] = &cmd_drawRectDefault;
	cmd_handlers[CMD_DRAW_RECT_THICK_DEFAULT] = &cmd_drawRectThickDefault;
	cmd_handlers[CMD_DRAW_RECT_THICK] = &cmd_drawRectThick;
	cmd_handlers[CMD_FILL_RECT] = &cmd_fillRect;
	cmd_handlers[CMD_FILL_RECT_DEFAULT] = &cmd_fillRectDefault;
	cmd_handlers[CMD_DRAW_CIRCLE_DEFAULT] = &cmd_drawCircleDefault;	
	cmd_handlers[CMD_DRAW_CIRCLE] = &cmd_drawCircle;	
	cmd_handlers[CMD_FILL_CIRCLE_DEFAULT] = &cmd_fillCircleDefault;
	cmd_handlers[CMD_FILL_CIRCLE] = &cmd_fillCircle;
	cmd_handlers[CMD_DRAW_CHAR] = &cmd_drawChar;
	cmd_handlers[CMD_FILL_CHAR] = &cmd_fillChar;
	cmd_handlers[CMD_DRAW_CHAR_XY] = &cmd_drawCharXY;
	cmd_handlers[CMD_FILL_CHAR_XY] = &cmd_fillCharXY;
	cmd_handlers[CMD_DRAW_STRING] = &cmd_drawString;
	cmd_handlers[CMD_FILL_STRING] = &cmd_fillString;	
	cmd_handlers[CMD_DRAW_STRING_XY] = &cmd_drawStringAnchor;
	cmd_handlers[CMD_FILL_STRING_XY] = &cmd_fillStringAnchor;
	cmd_handlers[CMD_SET_FILL_STRING_REGION] = &cmd_setFillStringRegion;
	cmd_handlers[CMD_DRAW_VERTICAL_SCROLLBAR] = &cmd_drawVerticalScrollbar;
	cmd_handlers[CMD_DRAW_PROGRESSBAR] = &cmd_drawProgressBar;
	cmd_handlers[CMD_LOAD_ICON] = &cmd_loadIcon;
	cmd_handlers[CMD_DRAW_ICON] = &cmd_drawIcon;
	cmd_handlers[CMD_FILL_ICON] = &cmd_fillIcon;
	cmd_handlers[CMD_LOAD_VGI] = &cmd_loadVgi;
	cmd_handlers[CMD_DRAW_VGI] = &cmd_drawVgi;
	cmd_handlers[CMD_DRAW_VGI_XY] = &cmd_drawVgiXY;	

	cmd_handlers[CMD_DEBUG] = &cmd_debug;
}


void CmdExec(uint8_t code) {
	if (code <= API_MAX_CODE) {
		cmd_handlers[code]();
	}
}