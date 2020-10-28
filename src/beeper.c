#include "beeper.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "general.h"

volatile uint16_t beeper_timer1value; 
volatile uint8_t beeper_duration;
volatile uint8_t beeperVolume = DEFAULT_BEEPER_VOLUME;

ISR(TIMER1_COMPA_vect) {
	OCR1A += beeper_timer1value;
	
	static uint8_t cnt = 0;
	if (cnt & 1) {
		OCR0B = beeperVolume;
	} else {
		OCR0B = 0;
	}
	cnt++;
//	if (beeper_duration) {
//		BEEPER_PORT ^= _BV(BEEPER_PIN);
//	}
}


void BeeperInit() {
	DDR(BEEPER_PORT) |= _BV(BEEPER_PIN); 
	BEEPER_PORT &= ~_BV(BEEPER_PIN);
	
	TCCR1A = 0;
	TCCR1B = _BV(CS12);	// divider for TC1 = 256
	TCCR1C = 0;
	
	TCCR0A = _BV(COM0A1) | _BV(COM0B1);	// set none-inverting mode for OC0B and OC0A
	TCCR0A |= _BV(WGM01) | _BV(WGM00);		// fast PWM
	TCCR0B = _BV(CS01);							// prescaler = 8 and start PWM
}

void BeeperStart(uint16_t f, uint8_t d) {
	beeper_timer1value = F_CPU/256/f;
	beeper_duration = d;
//	TCNT1 = 0;
	OCR1A = TCNT1 + beeper_timer1value;
	TIMSK1 |= _BV(OCIE1A);
}

void BeeperStop() {
	beeper_duration = 0;
	TIMSK1 &= ~_BV(OCIE1A);
	BEEPER_PORT &= ~_BV(BEEPER_PIN);
	OCR0B = 0;
}


void SetBeeperVolume(uint8_t volume) {
	beeperVolume = volume;
	//OCR0B = volume;
}