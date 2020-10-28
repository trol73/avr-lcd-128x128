/* 
 * File:   beeper.h
 * Author: Trol
 *
 * Created on March 19, 2016, 5:00 PM
 */

#ifndef _BEEPER_H_
#define _BEEPER_H_

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

#include "config.h"
#include "general.h"


#define beeper_on()			BEEPER_PORT |= _BV(BEEPER_PIN)
#define beeper_off()			BEEPER_PORT &= ~_BV(BEEPER_PIN)

//#define beeper_init()		DDRD |= _BV(5); PORTD &= ~_BV(5)

void BeeperInit();
void BeeperStart(uint16_t f, uint8_t d);
void BeeperStop();

void SetBeeperVolume(uint8_t volume);


#endif	// _BEEPER_H_

