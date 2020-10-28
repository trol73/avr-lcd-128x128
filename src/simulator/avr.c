#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;


//#define strlcpy strncpy
#define strlcpy_P strncpy

#pragma GCC diagnostic ignored "-Winvalid-source-encoding"


volatile uint8_t DDRA, DDRB, DDRC, DDRD, DDRE, DDRF, DDRG, PORTA, PORTB, PORTC, PORTD, PORTE, PORTF, PORTG, PINA, PINB, PINC, PIND, PINE, PINF, PING;
volatile uint8_t TIMSK, TIMSK1, TCNT0, TCNT1, TCCR1A, TCCR1B, OCR3BL, TCCR3A, TCCR0, OCR1A, ADMUX, ADCSRA, OCR2, ADC, TCCR2, TIFR, ICR1, TCCR3B, TWSR, TWBR, TWCR, TWDR;
volatile uint16_t OCR0A, OCR0B, OCR1B, OCR3B, TCCR1C, TCCR0A, TCCR0B;

volatile uint8_t UCSR0A, UCSR0B, UCSR0C, UDR0, UBRR0H, UBRR0L, SPDR, SPSR, SPCR; 
//uint8_t UCSR0A, UCSR0B, UCSR0C, UDR0, UDRIE0, UBRR0L, UBRR0H, U2X0, RXCIE0
 



void _delay_ms(double __ms) {
	usleep(__ms*1000);	
}

void _delay_us(double __us) {
	usleep(__us);	
}



char* itoa(int value, char *str, int radix) {
	char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign) {
        *str++ = '-';
        len++;
    }

    while (tp > tmp)
        *str++ = *--tp;

    return str;
}

#define BUFSIZE (sizeof(long) * 8 + 1)

char *ltoa(long N, char *str, int base) {
      register int i = 2;
      long uarg;
      char *tail, *head = str, buf[BUFSIZE];

      if (36 < base || 2 > base)
            base = 10;                    /* can only use 0-9, A-Z        */
      tail = &buf[BUFSIZE - 1];           /* last character position      */
      *tail-- = '\0';

      if (10 == base && N < 0L) {
            *head++ = '-';
            uarg    = -N;
      }
      else  uarg = N;

      if (uarg) {
            for (i = 1; uarg; ++i) {
                  register ldiv_t r;

                  r       = ldiv(uarg, base);
                  *tail-- = (char)(r.rem + ((9L < r.rem) ?
                                  ('A' - 10L) : '0'));
                  uarg    = r.quot;
            }
      }
      else  *tail-- = '0';

      memcpy(head, ++tail, i);
      return str;
}

void wdt_reset() {
	usleep(100);
}


