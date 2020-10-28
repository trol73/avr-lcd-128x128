/*
 * File:   lcd.h
 * Author: trol
 *
 * Created on December 15, 2016, 8:11 PM
 */

#ifndef _LCD_H_
#define _LCD_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <stdint.h>
#include <stdbool.h>



#define LCD_WIDTH	      128
#define LCD_HEIGHT	   128

// rrrrrggg:gggbbbbb
// Color definitions
#define COLOR_BLACK     0x0000
#define COLOR_BLUE      0x001F
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_CYAN      0x07FF
#define COLOR_MAGENTA   0xF81F
#define COLOR_YELLOW    0xFFE0
#define COLOR_WHITE     0xFFFF



void lcd_init();
void fillScreen(uint16_t color);
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void drawPixel(uint8_t x, uint8_t y, uint16_t color);
void drawHorizontalLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color);
void drawVerticalLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color);
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);
void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void drawRectThick(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t tx, uint8_t ty, uint16_t color);
void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color);
void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color);


void drawDataStart(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void drawDataFinish();
void drawDataNext(uint8_t hi, uint8_t lo);


#endif // _LCD_H_

