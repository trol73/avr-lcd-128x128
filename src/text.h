/*
 * File:   text.h
 * Author: trol
 *
 * Created on 18 dec 2016 ã., 11:28
 */

#ifndef _TEXT_H_
#define _TEXT_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <stdint.h>
#include <stdbool.h>

#define ANCHOR_LEFT			0
#define ANCHOR_RIGHT			1
#define ANCHOR_CENTER_H		2
#define ANCHOR_TOP			0
#define ANCHOR_BOTTOM		4
#define ANCHOR_CENTER_V		8
#define ANCHOR_FULL_LINE	16
#define ANCHOR_AUTOWRAP		32


typedef enum {
	STANG,
	MIKRO,
	GLCD_UTILS
} font_table_type_t;


typedef struct {
	const char *font_table;
	uint8_t width;
	uint8_t height;
	uint8_t start_char;
	uint8_t end_char;
	font_table_type_t table_type;
} font_config_t;

extern font_config_t font_current;

void setFont(PGM_P font_table, uint8_t width, uint8_t height, uint8_t start_char, uint8_t end_char, font_table_type_t type);
uint8_t drawCharXY(uint8_t x, uint8_t y, uint8_t c, uint16_t color);
uint8_t fillCharXY(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t background);
uint8_t getCharWidth(uint8_t ch);

#endif // _TEXT_H_

