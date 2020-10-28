/* 
 * File:   gui.h
 * Author: trol
 *
 * Created on 29 ÿםגאנÿ 2017 ד., 21:03
 */

#ifndef _GUI_H_
#define _GUI_H_

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

#define GUI_FLAG_FIRST_PAINT		1

#define GUI_ITEM_END					0
#define GUI_ITEM_SCREEN				1
#define GUI_ITEM_LIST				2



#define GUI_SCREEN_OPTION_HAS_TITLE			1
#define GUI_SCREEN_OPTION_HAS_BUTTONS		2


typedef struct {
	uint16_t data_block_size;
	uint8_t options_mask;
} gui_screen_t;


typedef struct {
	uint16_t data_block_size;
	uint8_t list_size;
	uint8_t screen_y;
	uint16_t first_visible_offset;
	uint8_t first_visible_index;
	uint8_t options_mask;
} gui_menu_t;



void guiDraw();
bool guiKeyHandler(uint8_t key);


extern uint8_t gui_flags;

#endif // _GUI_H_

