/*
 * File:   api_codes.h
 * Author: trol
 *
 * Created on December 16, 2016, 8:37 PM
 */

#ifndef _API_CODES_H_
#define _API_CODES_H_

#define CMD_GET_VERSION								0			// 0 bytes
#define CMD_SYNC									1			// 1 bytes
#define CMD_START_BOOTLOADER							2			// 0 bytes
#define CMD_SET_BACKLIGHT_TIME						3			// 2 bytes
#define CMD_SET_BACKLIGHT_LEVEL_LOW					4			// 1 byte
#define CMD_SET_BACKLIGHT_LEVEL_HIGH					5			// 1 byte
#define CMD_SET_BEEPER_VOLUME							6			// 1 byte
#define CMD_SET_KEYBOARD_BEEP							7			// 1 bytes
#define CMD_READ_KEYBOARD							8			// 0 bytes
#define CMD_BEEP									9			// 3 bytes
#define CMD_PLAY_MELODY								10			// variable
#define CMD_SET_STRING_INTERVAL_X						11			// 1 bytes
#define CMD_SET_STRING_INTERVAL_Y						12			// 1 bytes
#define CMD_SET_COLOR								13			// 2 bytes
#define CMD_SET_BACKGROUND							14			// 2 bytes
#define CMD_SET_FONT								15			// 1 byte
#define CMD_DRAW_PIXEL								16			// 2 bytes
#define CMD_DRAW_LINE_DEFAULT							17			// 4 bytes
#define CMD_FILL_SCREEN								18			// 2 bytes
#define CMD_DRAW_LINE								19			// 6 bytes
#define CMD_DRAW_LINE_TO								20			// 2 bytes
#define CMD_DRAW_RECT_DEFAULT							21			// 4 bytes
#define CMD_DRAW_RECT								22			// 6 bytes
#define CMD_DRAW_RECT_THICK_DEFAULT					23			// 6 bytes
#define CMD_DRAW_RECT_THICK							24			// 8 bytes
#define CMD_FILL_RECT								25			// 6 bytes
#define CMD_FILL_RECT_DEFAULT							26			// 4 bytes
#define CMD_DRAW_CIRCLE_DEFAULT						27			// 3 bytes
#define CMD_DRAW_CIRCLE								28			// 5 bytes
#define CMD_FILL_CIRCLE_DEFAULT						29			// 3 bytes
#define CMD_FILL_CIRCLE								30			// 5 bytes
#define CMD_DRAW_CHAR								31			// 1 bytes
#define CMD_FILL_CHAR								32			// 1 byte
#define CMD_DRAW_CHAR_XY								33			// 3 bytes
#define CMD_FILL_CHAR_XY								34			// 3 bytes
#define CMD_DRAW_STRING								35			// variable
#define CMD_FILL_STRING								36			// variable
#define CMD_DRAW_STRING_XY							37			// variable
#define CMD_SET_FILL_STRING_REGION						38			// 2 bytes
#define CMD_FILL_STRING_XY							39			// variable


#define CMD_DRAW_VERTICAL_SCROLLBAR					40			// 9 bytes
#define CMD_DRAW_PROGRESSBAR							41			// 5 bytes

#define CMD_LOAD_ICON								42			// variable
#define CMD_DRAW_ICON								43			// 3 bytes
#define CMD_FILL_ICON								44			// 3 bytes

#define CMD_LOAD_VGI								45			// variable
#define CMD_DRAW_VGI								46			// 1 byte
#define CMD_DRAW_VGI_XY								47			// 3 byte


#define CMD_DEBUG									48			// 0 bytes

#define API_MAX_CODE								48



#endif // _API_CODES_H_

