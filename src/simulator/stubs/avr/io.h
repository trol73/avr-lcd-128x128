#pragma GCC diagnostic ignored "-Winvalid-source-encoding"

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

#include "avrdef.h"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;


extern volatile uint8_t DDRA, DDRB, DDRC, DDRD, DDRE, PORTA, PORTB, PORTC, PORTD, PORTE, PINA, PINB, PINC, PIND, PINE;
extern volatile uint8_t TIMSK, TIMSK1, TCNT0, TCNT1, TCCR1A, TCCR1B, OCR3BL, TCCR3A, TCCR0, OCR1A, ADMUX, ADCSRA, OCR2, ADC, TCCR2, TIFR, ICR1, TCCR3B, TWSR, TWBR, TWCR, TWDR;
extern volatile uint16_t OCR0A, OCR0B, OCR1B, OCR3B, SPDR, SPSR, SPCR;
extern volatile uint8_t UCSR0A, UCSR0B, UCSR0C, UDR0, UBRR0H, UBRR0L, TCCR1C, TCCR0A, TCCR0B;


/* 2-wire Control Register - TWCR */
#define    TWINT        7
#define    TWEA         6
#define    TWSTA        5
#define    TWSTO        4
#define    TWWC         3
#define    TWEN         2
#define    TWIE         0

/* 2-wire Address Register - TWAR */
#define    TWA6         7
#define    TWA5         6
#define    TWA4         5
#define    TWA3         4
#define    TWA2         3
#define    TWA1         2
#define    TWA0         1
#define    TWGCE        0

/* 2-wire Status Register - TWSR */
#define    TWS7         7
#define    TWS6         6
#define    TWS5         5
#define    TWS4         4
#define    TWS3         3
#define    TWPS1        1
#define    TWPS0        0

/* External Memory Control Register A - XMCRA */
#define    SRL2         6
#define    SRL1         5
#define    SRL0         4
#define    SRW01        3
#define    SRW00        2
#define    SRW11        1

/* External Memory Control Register B - XMCRA */
#define    XMBK         7
#define    XMM2         2
#define    XMM1         1
#define    XMM0         0

/* XDIV Divide control register - XDIV */
#define    XDIVEN       7
#define    XDIV6        6
#define    XDIV5        5
#define    XDIV4        4
#define    XDIV3        3
#define    XDIV2        2
#define    XDIV1        1
#define    XDIV0        0

/* RAM Page Z select register - RAMPZ */
#define     RAMPZ0      0

/* External Interrupt Control Register A - EICRA */
#define    ISC31        7
#define    ISC30        6
#define    ISC21        5
#define    ISC20        4
#define    ISC11        3
#define    ISC10        2
#define    ISC01        1
#define    ISC00        0

/* External Interrupt Control Register B - EICRB */
#define    ISC71        7
#define    ISC70        6
#define    ISC61        5
#define    ISC60        4
#define    ISC51        3
#define    ISC50        2
#define    ISC41        1
#define    ISC40        0

/* Store Program Memory Control Register - SPMCSR, SPMCR */
#define    SPMIE        7
#define    RWWSB        6
#define    RWWSRE       4
#define    BLBSET       3
#define    PGWRT        2
#define    PGERS        1
#define    SPMEN        0

/* External Interrupt MaSK register - EIMSK */
#define    INT7         7
#define    INT6         6
#define    INT5         5
#define    INT4         4
#define    INT3         3
#define    INT2         2
#define    INT1         1
#define    INT0         0

/* External Interrupt Flag Register - EIFR */
#define    INTF7        7
#define    INTF6        6
#define    INTF5        5
#define    INTF4        4
#define    INTF3        3
#define    INTF2        2
#define    INTF1        1
#define    INTF0        0

/* Timer/Counter Interrupt MaSK register - TIMSK */
#define    OCIE2        7
#define    TOIE2        6
#define    TICIE1       5
#define    OCIE1A       4
#define    OCIE1B       3
#define    TOIE1        2
#define    OCIE0        1
#define    TOIE0        0

/* Timer/Counter Interrupt Flag Register - TIFR */
#define    OCF2         7
#define    TOV2         6
#define    ICF1         5
#define    OCF1A        4
#define    OCF1B        3
#define    TOV1         2
#define    OCF0         1
#define    TOV0         0

/* Extended Timer Interrupt MaSK register - ETIMSK */
#define    TICIE3       5
#define    OCIE3A       4
#define    OCIE3B       3
#define    TOIE3        2
#define    OCIE3C       1
#define    OCIE1C       0

/* Extended Timer Interrupt Flag Register - ETIFR */
#define    ICF3         5
#define    OCF3A        4
#define    OCF3B        3
#define    TOV3         2
#define    OCF3C        1
#define    OCF1C        0

/* MCU general Control Register - MCUCR */
#define    SRE          7
#define    SRW          6
#define    SRW10        6       /* new name in datasheet (2467E-AVR-05/02) */
#define    SE           5
#define    SM1          4
#define    SM0          3
#define    SM2          2
#define    IVSEL        1
#define    IVCE         0

/* MCU Status Register - MCUSR, MCUCSR */
#define    JTD          7
#define    JTRF         4
#define    WDRF         3
#define    BORF         2
#define    EXTRF        1
#define    PORF         0

/* Timer/Counter Control Register (generic) */
#define    FOC          7
#define    WGM0         6
#define    COM1         5
#define    COM0         4
#define    WGM1         3
#define    CS2          2
#define    CS1          1
#define    CS0          0

/* Timer/Counter 0 Control Register - TCCR0 */
#define    FOC0         7
#define    WGM00        6
#define    COM01        5
#define    COM00        4
#define    WGM01        3
#define    CS02         2
#define    CS01         1
#define    CS00         0

/* Timer/Counter 2 Control Register - TCCR2 */
#define    FOC2         7
#define    WGM20        6
#define    COM21        5
#define    COM20        4
#define    WGM21        3
#define    CS22         2
#define    CS21         1
#define    CS20         0

/* Timer/Counter 0 Asynchronous Control & Status Register - ASSR */
#define    AS0          3
#define    TCN0UB       2
#define    OCR0UB       1
#define    TCR0UB       0

/* Timer/Counter Control Register A (generic) */
#define    COMA1        7
#define    COMA0        6
#define    COMB1        5
#define    COMB0        4
#define    COMC1        3
#define    COMC0        2
#define    WGMA1        1
#define    WGMA0        0

/* Timer/Counter 1 Control and Status Register A - TCCR1A */
#define    COM1A1       7
#define    COM1A0       6
#define    COM1B1       5
#define    COM1B0       4
#define    COM1C1       3
#define    COM1C0       2
#define    WGM11        1
#define    WGM10        0

/* Timer/Counter 3 Control and Status Register A - TCCR3A */
#define    COM3A1       7
#define    COM3A0       6
#define    COM3B1       5
#define    COM3B0       4
#define    COM3C1       3
#define    COM3C0       2
#define    WGM31        1
#define    WGM30        0

/* Timer/Counter Control and Status Register B (generic) */
#define    ICNC         7
#define    ICES         6
#define    WGMB3        4
#define    WGMB2        3
#define    CSB2         2
#define    CSB1         1
#define    CSB0         0

/* Timer/Counter 1 Control and Status Register B - TCCR1B */
#define    ICNC1        7
#define    ICES1        6
#define    WGM13        4
#define    WGM12        3
#define    CS12         2
#define    CS11         1
#define    CS10         0

/* Timer/Counter 3 Control and Status Register B - TCCR3B */
#define    ICNC3        7
#define    ICES3        6
#define    WGM33        4
#define    WGM32        3
#define    CS32         2
#define    CS31         1
#define    CS30         0

/* Timer/Counter Control Register C (generic) */
#define    FOCA         7
#define    FOCB         6
#define    FOCC         5

/* Timer/Counter 3 Control Register C - TCCR3C */
#define    FOC3A        7
#define    FOC3B        6
#define    FOC3C        5

/* Timer/Counter 1 Control Register C - TCCR1C */
#define    FOC1A        7
#define    FOC1B        6
#define    FOC1C        5

/* On-chip Debug Register - OCDR */
#define    IDRD         7
#define    OCDR7        7
#define    OCDR6        6
#define    OCDR5        5
#define    OCDR4        4
#define    OCDR3        3
#define    OCDR2        2
#define    OCDR1        1
#define    OCDR0        0

/* Watchdog Timer Control Register - WDTCR */
#define    WDCE         4
#define    WDE          3
#define    WDP2         2
#define    WDP1         1
#define    WDP0         0

/* 
   The ADHSM bit has been removed from all documentation, 
   as being not needed at all since the comparator has proven 
   to be fast enough even without feeding it more power.
*/

/* Special Function I/O Register - SFIOR */
#define    TSM          7
#define    ACME         3
#define    PUD          2
#define    PSR0         1
#define    PSR321       0

/* SPI Status Register - SPSR */
#define    SPIF         7
#define    WCOL         6
#define    SPI2X        0

/* SPI Control Register - SPCR */
#define    SPIE         7
#define    SPE          6
#define    DORD         5
#define    MSTR         4
#define    CPOL         3
#define    CPHA         2
#define    SPR1         1
#define    SPR0         0

/* USART Register C (generic) */
#define    UMSEL        6
#define    UPM1         5
#define    UPM0         4
#define    USBS         3
#define    UCSZ1        2
#define    UCSZ0        1
#define    UCPOL        0

/* USART1 Register C - UCSR1C */
#define    UMSEL1       6
#define    UPM11        5
#define    UPM10        4
#define    USBS1        3
#define    UCSZ11       2
#define    UCSZ10       1
#define    UCPOL1       0

/* USART0 Register C - UCSR0C */
#define    UMSEL0       6
#define    UPM01        5
#define    UPM00        4
#define    USBS0        3
#define    UCSZ01       2
#define    UCSZ00       1
#define    UCPOL0       0

/* USART Status Register A (generic) */
#define    RXC          7
#define    TXC          6
#define    UDRE         5
#define    FE           4
#define    DOR          3
#define    UPE          2
#define    U2X          1
#define    MPCM         0

/* USART1 Status Register A - UCSR1A */
#define    RXC1         7
#define    TXC1         6
#define    UDRE1        5
#define    FE1          4
#define    DOR1         3
#define    UPE1         2
#define    U2X1         1
#define    MPCM1        0

/* USART0 Status Register A - UCSR0A */
#define    RXC0         7
#define    TXC0         6
#define    UDRE0        5
#define    FE0          4
#define    DOR0         3
#define    UPE0         2
#define    U2X0         1
#define    MPCM0        0

/* USART Control Register B (generic) */
#define    RXCIE        7
#define    TXCIE        6
#define    UDRIE        5
#define    RXEN         4
#define    TXEN         3
#define    UCSZ         2
#define    UCSZ2        2       /* new name in datasheet (2467E-AVR-05/02) */
#define    RXB8         1
#define    TXB8         0

/* USART1 Control Register B - UCSR1B */
#define    RXCIE1       7
#define    TXCIE1       6
#define    UDRIE1       5
#define    RXEN1        4
#define    TXEN1        3
#define    UCSZ12       2
#define    RXB81        1
#define    TXB81        0

/* USART0 Control Register B - UCSR0B */
#define    RXCIE0       7
#define    TXCIE0       6
#define    UDRIE0       5
#define    RXEN0        4
#define    TXEN0        3
#define    UCSZ02       2
#define    RXB80        1
#define    TXB80        0

/* Analog Comparator Control and Status Register - ACSR */
#define    ACD          7
#define    ACBG         6
#define    ACO          5
#define    ACI          4
#define    ACIE         3
#define    ACIC         2
#define    ACIS1        1
#define    ACIS0        0

/* ADC Control and status register - ADCSRA */
#define    ADEN         7
#define    ADSC         6
#define    ADFR         5
#define    ADIF         4
#define    ADIE         3
#define    ADPS2        2
#define    ADPS1        1
#define    ADPS0        0

/* ADC Multiplexer select - ADMUX */
#define    REFS1        7
#define    REFS0        6
#define    ADLAR        5
#define    MUX4         4
#define    MUX3         3
#define    MUX2         2
#define    MUX1         1
#define    MUX0         0

/* Port A Data Register - PORTA */
#define    PA7       7
#define    PA6       6
#define    PA5       5
#define    PA4       4
#define    PA3       3
#define    PA2       2
#define    PA1       1
#define    PA0       0

/* Port A Data Direction Register - DDRA */
#define    DDA7         7
#define    DDA6         6
#define    DDA5         5
#define    DDA4         4
#define    DDA3         3
#define    DDA2         2
#define    DDA1         1
#define    DDA0         0

/* Port A Input Pins - PINA */
#define    PINA7        7
#define    PINA6        6
#define    PINA5        5
#define    PINA4        4
#define    PINA3        3
#define    PINA2        2 
#define    PINA1        1
#define    PINA0        0

/* Port B Data Register - PORTB */
#define    PB7       7
#define    PB6       6
#define    PB5       5
#define    PB4       4
#define    PB3       3
#define    PB2       2
#define    PB1       1
#define    PB0       0

/* Port B Data Direction Register - DDRB */
#define    DDB7         7
#define    DDB6         6
#define    DDB5         5
#define    DDB4         4
#define    DDB3         3
#define    DDB2         2
#define    DDB1         1
#define    DDB0         0

/* Port B Input Pins - PINB */
#define    PINB7        7
#define    PINB6        6
#define    PINB5        5
#define    PINB4        4
#define    PINB3        3
#define    PINB2        2 
#define    PINB1        1
#define    PINB0        0

/* Port C Data Register - PORTC */
#define    PC7       7
#define    PC6       6
#define    PC5       5
#define    PC4       4
#define    PC3       3
#define    PC2       2
#define    PC1       1
#define    PC0       0

/* Port C Data Direction Register - DDRC */
#define    DDC7         7
#define    DDC6         6
#define    DDC5         5
#define    DDC4         4
#define    DDC3         3
#define    DDC2         2
#define    DDC1         1
#define    DDC0         0

/* Port C Input Pins - PINC */
#define    PINC7        7
#define    PINC6        6
#define    PINC5        5
#define    PINC4        4
#define    PINC3        3
#define    PINC2        2 
#define    PINC1        1
#define    PINC0        0

/* Port D Data Register - PORTD */
#define    PD7       7
#define    PD6       6
#define    PD5       5
#define    PD4       4
#define    PD3       3
#define    PD2       2
#define    PD1       1
#define    PD0       0

/* Port D Data Direction Register - DDRD */
#define    DDD7         7
#define    DDD6         6
#define    DDD5         5
#define    DDD4         4
#define    DDD3         3
#define    DDD2         2
#define    DDD1         1
#define    DDD0         0

/* Port D Input Pins - PIND */
#define    PIND7        7
#define    PIND6        6
#define    PIND5        5
#define    PIND4        4
#define    PIND3        3
#define    PIND2        2 
#define    PIND1        1
#define    PIND0        0

/* Port E Data Register - PORTE */
#define    PE7       7
#define    PE6       6
#define    PE5       5
#define    PE4       4
#define    PE3       3
#define    PE2       2
#define    PE1       1
#define    PE0       0

/* Port E Data Direction Register - DDRE */
#define    DDE7         7
#define    DDE6         6
#define    DDE5         5
#define    DDE4         4
#define    DDE3         3
#define    DDE2         2
#define    DDE1         1
#define    DDE0         0

/* Port E Input Pins - PINE */
#define    PINE7        7
#define    PINE6        6
#define    PINE5        5
#define    PINE4        4
#define    PINE3        3
#define    PINE2        2 
#define    PINE1        1
#define    PINE0        0

/* Port F Data Register - PORTF */
#define    PF7       7
#define    PF6       6
#define    PF5       5
#define    PF4       4
#define    PF3       3
#define    PF2       2
#define    PF1       1
#define    PF0       0

/* Port F Data Direction Register - DDRF */
#define    DDF7         7
#define    DDF6         6
#define    DDF5         5
#define    DDF4         4
#define    DDF3         3
#define    DDF2         2
#define    DDF1         1
#define    DDF0         0

/* Port F Input Pins - PINF */
#define    PINF7        7
#define    PINF6        6
#define    PINF5        5
#define    PINF4        4
#define    PINF3        3
#define    PINF2        2 
#define    PINF1        1
#define    PINF0        0

/* Port G Data Register - PORTG */
#define    PG4       4
#define    PG3       3
#define    PG2       2
#define    PG1       1
#define    PG0       0

/* Port G Data Direction Register - DDRG */
#define    DDG4         4
#define    DDG3         3
#define    DDG2         2
#define    DDG1         1
#define    DDG0         0

/* Port G Input Pins - PING */
#define    PING4        4
#define    PING3        3
#define    PING2        2 
#define    PING1        1
#define    PING0        0

/* EEPROM Control Register */
#define    EERIE        3
#define    EEMWE        2
#define    EEWE         1
#define    EERE         0

/* Constants */
#define SPM_PAGESIZE 256
#define RAMEND     0x10FF     /* Last On-Chip SRAM Location */
#define XRAMEND    0xFFFF
#define E2END      0x0FFF
#define E2PAGESIZE 8
#define FLASHEND   0x1FFFF


#define OCIE2B 2
#define COM0A1 7
#define COM0B1 6
