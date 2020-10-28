/*
 * File:   keyboard.h
 * Author: Trol
 *
 * Created on March 19, 2016, 3:30 PM
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>


#define KEY_UP			0
#define KEY_DOWN		1
#define KEY_LEFT		2
#define KEY_RIGHT		3
#define KEY_ENTER		4


//inline bool key_down_pressed() {
//	return (PINC & _BV(2)) == 0;
//}
//
//inline bool key_up_pressed() {
//	return (PINC & _BV(4)) == 0;
//}
//
//inline bool key_left_pressed() {
//	return (PINC & _BV(1)) == 0;
//}
//
//inline bool key_right_pressed() {
//	return (PINC & _BV(5)) == 0;
//}
//
//inline bool key_enter_pressed() {
//	return (PINC & _BV(3)) == 0;
//}
//
//
//inline bool key_any_pressed() {
//	return (PINC & (_BV(1)|_BV(2)|_BV(3)|_BV(4)|_BV(5))) != (_BV(1)|_BV(2)|_BV(3)|_BV(4)|_BV(5));
//}


void KeyboardInit();
void KeyboardCheck();
bool KeyboardRepeated(uint8_t key);

void KeyboardBeep(bool beep);
bool IsKeyboardBeep();
uint8_t KeyboardGetRepeatCount(uint8_t key);

void BacklightDisable();
void BacklightEnable();
void SetHighlightLevelHigh(uint8_t level);
void SetHighlightLevelLow(uint8_t level);

extern volatile uint16_t screenBacklightCounter;
extern volatile uint16_t keyboardAutoBacklightTime;

#endif	// _KEYBOARD_H

