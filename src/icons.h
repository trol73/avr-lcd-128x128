/*
 * File:   icons.h
 * Author: trol
 *
 * Created on September 6, 2017, 21:40 PM
 */

#ifndef _ICONS_H_
#define _ICONS_H_

#include "config.h"
#include "general.h"

#include <stdint.h>
#include <stdbool.h>

uint8_t loadIcon(uint8_t clearFrom, uint8_func_t loadFunc);
void drawIcon(uint8_t id, uint8_t x, uint8_t y, uint16_t color);
void fillIcon(uint8_t id, uint8_t x, uint8_t y, uint16_t color, uint16_t background);

void drawBitmap8x8(uint8_t x, uint8_t y, const uint8_t* ptr, uint16_t color);
void fillBitmap8x8(uint8_t x, uint8_t y, const uint8_t* ptr, uint16_t color, uint16_t background);



#endif // _ICONS_H_

