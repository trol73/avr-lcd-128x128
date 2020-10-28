/* 
 * File:   gui_controls.h
 * Author: trol
 *
 * Created on 31 ÿםגאנÿ 2017 ד., 20:22
 */

#ifndef _GUI_CONTROLS_H_
#define _GUI_CONTROLS_H_

#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "gui_scheme.h"


void drawVerticalScrollbar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t itemIndex, uint16_t numberOfItems, uint8_t interval);
void drawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t progress);


#endif // _GUI_CONTROLS_H_

