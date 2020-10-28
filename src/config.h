/*
 * File:   config.h
 * Author: trol
 *
 * Created on December 15, 2016, 8:10 PM
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CONTROLLER_MOSI_PORT	PORTB
#define CONTROLLER_MOSI_PIN	3

#define CONTROLLER_SCK_PORT	PORTB
#define CONTROLLER_SCK_PIN	5

#define CONTROLLER_DC_PORT	PORTB
#define CONTROLLER_DC_PIN	2	//0

#define CONTROLLER_RST_PORT	PORTB
#define CONTROLLER_RST_PIN	1	//2

#define CONTROLLER_CS_PORT	PORTB
#define CONTROLLER_CS_PIN	0	//1

#define BEEPER_PORT		PORTD
#define BEEPER_PIN		5

#define HIGHLIGHT_PORT	PORTD
#define HIGHLIGHT_PIN	6



#define UART_BAUD_RATE						57600

#define KEYBOARD_BEEP_FREQUENCY			2000
#define KEYBOARD_BEEP_DURATION			3

#define KEY_PRESS_TIME						5
#define KEY_REPEAT_TIME						50

//#define DEFAULT_DISPLAY_CONTRAST			65
#define DEFAULT_HIGHLIGHT_TIME			0xffff
#define DEFAULT_HIGHLIGHT_LEVEL_HIGH	0xff
#define DEFAULT_HIGHLIGHT_LEVEL_LOW		0

#define DEFAULT_BEEPER_VOLUME				128

// смещение координат (может быть разным у разных дисплеев)
#define LCD_DELTA_X					2
#define LCD_DELTA_Y					1

//#define LCD_DELTA_X					0
//#define LCD_DELTA_Y					0


#define GUI_DATA_SIZE			1024


#endif // _CONFIG_H_

