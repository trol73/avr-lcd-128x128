#include "lcd.h"

#include "config.h"
#include "general.h"
#include "simulator/display.h"

#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//#define set_dc(v) {if (v) CONTROLLER_DC_PORT |= _BV(CONTROLLER_DC_PIN); else CONTROLLER_DC_PORT &= ~_BV(CONTROLLER_DC_PIN);}
//#define set_reset(v) {if (v) CONTROLLER_RESET_PORT |= _BV(CONTROLLER_RESET_PIN); else CONTROLLER_RESET_PORT &= ~_BV(CONTROLLER_RESET_PIN);}
//#define set_cs(v) {if (v) CONTROLLER_CS_PORT |= _BV(CONTROLLER_CS_PIN); else CONTROLLER_CS_PORT &= ~_BV(CONTROLLER_CS_PIN);}

#define GLCD_DESELECT()   sbi(CONTROLLER_CS_PORT,CONTROLLER_CS_PIN)
#define GLCD_SELECT()     cbi(CONTROLLER_CS_PORT,CONTROLLER_CS_PIN)
#define GLCD_DC_LOW()     cbi(CONTROLLER_DC_PORT,CONTROLLER_DC_PIN)
#define GLCD_DC_HIGH()    sbi(CONTROLLER_DC_PORT,CONTROLLER_DC_PIN)
#define GLCD_RESET_LOW()  cbi(CONTROLLER_RST_PORT,CONTROLLER_RST_PIN)
#define GLCD_RESET_HIGH() sbi(CONTROLLER_RST_PORT,CONTROLLER_RST_PIN)

#define GLCD_A0_LOW()     cbi(CONTROLLER_A0_PORT,CONTROLLER_A0_PIN)
#define GLCD_A0_HIGH()    sbi(CONTROLLER_A0_PORT,CONTROLLER_A0_PIN)



// some flags for initR() :(
#define INITR_GREENTAB 0x0
#define INITR_REDTAB   0x1
#define INITR_BLACKTAB   0x2

#define INITR_18GREENTAB    INITR_GREENTAB
#define INITR_18REDTAB      INITR_REDTAB
#define INITR_18BLACKTAB    INITR_BLACKTAB
#define INITR_144GREENTAB   0x1

#define ST7735_TFTWIDTH  128
// for 1.44" display
#define ST7735_TFTHEIGHT_144 128
// for 1.8" display
#define ST7735_TFTHEIGHT_18  160

#define ST7735_NOP     0x00	// No Operation
#define ST7735_SWRESET 0x01	// Software reset
#define ST7735_RDDID   0x04	// Read Display ID
#define ST7735_RDDST   0x09	// Read Display Status

#define ST7735_SLPIN   0x10	// Sleep in & booster off
#define ST7735_SLPOUT  0x11	// Sleep out & booster on
#define ST7735_PTLON   0x12	// Partial mode on
#define ST7735_NORON   0x13	// Partial off (Normal)

#define ST7735_INVOFF  0x20	// Display inversion off
#define ST7735_INVON   0x21	// Display inversion on
#define ST7735_GAMSET  0x26	// Gamma curve select
#define ST7735_DISPOFF 0x28	// Display off
#define ST7735_DISPON  0x29	// Display on
#define ST7735_CASET   0x2A	// Column address set
#define ST7735_RASET   0x2B	// Row address set
#define ST7735_RAMWR   0x2C	// Memory write
#define ST7735_RAMRD   0x2E	// Memory read

#define ST7735_PTLAR   0x30	// Partial start/end address set
#define ST7735_MADCTL  0x36	// Memory data access control
#define ST7735_COLMOD  0x3A	// Interface pixel format

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1



#define GLCD_RESET_TIME	50


#define swap(a, b) { uint8_t t = a; a = b; b = t; }

void lcd_reset(void) {
	// Toggle RST low to reset. Minimum pulse 100ns on datasheet.
	GLCD_SELECT();
	GLCD_RESET_LOW();
	_delay_ms(GLCD_RESET_TIME);
	GLCD_RESET_HIGH();
	GLCD_DESELECT();
}

void lcd_spi_write(uint8_t c) {
	GLCD_SELECT();
	SPDR = c;
	uint8_t cnt = 0;
	// wait until transmission is complete
	while (!(SPSR & (1<<SPIF))) {
		cnt++;
		if (cnt == 0xff) {
			//glcd_reset();
			break;
		}
	}
	GLCD_DESELECT();
}

static inline void lcd_spi_write_fast(uint8_t c) {
	SPDR = c;

	uint8_t cnt = 0;
	// wait until transmission is complete
	while (!(SPSR & _BV(SPIF))) {
		cnt++;
		if (cnt == 0xff) {
		//if (cnt == 0) {
			//glcd_reset();
			break;
		}
	}
}

void lcd_command(uint8_t c) {
	GLCD_DC_LOW();
	lcd_spi_write(c);
}

void lcd_data(uint8_t c) {
	GLCD_DC_HIGH();
	lcd_spi_write(c);
}
static inline void lcd_data_w(uint16_t c) {
	GLCD_DC_HIGH();
	lcd_spi_write(c >> 8);
	lcd_spi_write(c & 0xff);
}

static inline void lcd_data_w_fast(uint16_t c) {
	GLCD_DC_HIGH();
	lcd_spi_write_fast(c >> 8);
	lcd_spi_write_fast(c & 0xff);
}

#define DELAY 0x80
static const uint8_t PROGMEM
/*
  Bcmd[] = {                  // Initialization commands for 7735B screens
    18,                       // 18 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, no args, w/delay
      50,                     //     50 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, no args, w/delay
      255/10,                    //     255 = 500 ms delay
    ST7735_COLMOD , 1+DELAY,  //  3: Set color mode, 1 arg + delay:
      0x05,                   //     16-bit color
      10,                     //     10 ms delay
    ST7735_FRMCTR1, 3+DELAY,  //  4: Frame rate control, 3 args + delay:
      0x00,                   //     fastest refresh
      0x06,                   //     6 lines front porch
      0x03,                   //     3 lines back porch
      10,                     //     10 ms delay
    ST7735_MADCTL , 1      ,  //  5: Memory access ctrl (directions), 1 arg:
      0x08,                   //     Row addr/col addr, bottom to top refresh
    ST7735_DISSET5, 2      ,  //  6: Display settings #5, 2 args, no delay:
      0x15,                   //     1 clk cycle nonoverlap, 2 cycle gate
                              //     rise, 3 cycle osc equalize
      0x02,                   //     Fix on VTL
    ST7735_INVCTR , 1      ,  //  7: Display inversion control, 1 arg:
      0x0,                    //     Line inversion
    ST7735_PWCTR1 , 2+DELAY,  //  8: Power control, 2 args + delay:
      0x02,                   //     GVDD = 4.7V
      	//0x10,				//     GVDD = 4.0V
      0x70,                   //     1.0uA
      10,                     //     10 ms delay
    ST7735_PWCTR2 , 1      ,  //  9: Power control, 1 arg, no delay:
      0x05,                   //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3 , 2      ,  // 10: Power control, 2 args, no delay:
      0x01,                   //     Opamp current small
      0x02,                   //     Boost frequency
    ST7735_VMCTR1 , 2+DELAY,  // 11: Power control, 2 args + delay:
      0x3C,                   //     VCOMH = 4V
      0x38,                   //     VCOML = -1.1V
      10,                     //     10 ms delay
    ST7735_PWCTR6 , 2      ,  // 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735_GMCTRP1,16      ,  // 13: Magical unicorn dust, 16 args, no delay:
      0x09, 0x16, 0x09, 0x20, //     (seriously though, not sure what
      0x21, 0x1B, 0x13, 0x19, //      these config values represent)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1,16+DELAY,  // 14: Sparkles and rainbows, 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E, //     (ditto)
      0x22, 0x1D, 0x18, 0x1E,
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      10,                     //     10 ms delay
    ST7735_CASET  , 4      ,  // 15: Column addr set, 4 args, no delay:
//      0x00, 0x02,             //     XSTART = 2
0x00, 0x01,             //     XSTART = 1
//      0x00, 0x81,             //     XEND = 129
0x00, 0x82,             //     XEND = 129
    ST7735_RASET  , 4      ,  // 16: Row addr set, 4 args, no delay:
//      0x00, 0x02,             //     XSTART = 1
0x00, 0x01,             //     XSTART = 1
//      0x00, 0x81,             //     XEND = 160
0x00, 0x82,             //     XEND = 160
    ST7735_NORON  ,   DELAY,  // 17: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,   DELAY,  // 18: Main screen turn on, no args, w/delay
      255/10 },                  //     255 = 500 ms delay
*/






  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
        0x08,				  //     Row addr/col addr, bottom to top refresh
      //0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 },                 //     16-bit color

/*
  Rcmd2green[] = {            // Init for 7735R, part 2 (green tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 0
      0x00, 0x7F+0x02,        //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,             //     XSTART = 0
      0x00, 0x9F+0x01 },      //     XEND = 159
*/
  Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x80,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
		0x00, 0x80,             //     XEND = 128
   },           //     XEND = 159
/*
  Rcmd2green144[] = {              // Init for 7735R, part 2 (green 1.44 tab)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F },           //     XEND = 127
*/
  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100					//     100 ms delay
};


static void commandList(const uint8_t *addr) {
	uint8_t numCommands = pgm_read_byte(addr++);   // Number of commands to follow
	while(numCommands--) {                 // For each command...
		lcd_command(pgm_read_byte(addr++)); //   Read, issue command
		uint8_t numArgs = pgm_read_byte(addr++);    //   Number of args to follow
		// TODO bool
		uint16_t ms = numArgs & DELAY;          //   If hibit set, delay follows args
		numArgs &= ~DELAY;                   //   Mask out delay bit
		while(numArgs--) {                   //   For each argument...
			lcd_data(pgm_read_byte(addr++));  //     Read, issue argument
		}

		if (ms) {
			ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
			if(ms == 255) ms = 500;     // If 255, delay for 500 ms
			_delay_ms(ms);
		}
	}
}



void lcd_init() {
	 // Set up SPI for AVR8
	 // Note: AVR's SS pin must be set to output, regardless of whether we actually use it. This is a requirement of SPI master mode.
//	sbi(DDR(AVR_SS_PORT),AVR_SS_PIN);

	/*
	 *  Set MOSI, Master SS, SCK to output (otherwise SPI won't work)
	 *  Must be done even if native SS pin not used
	 */
	sbi(DDR(CONTROLLER_MOSI_PORT),CONTROLLER_MOSI_PIN);
	sbi(DDR(CONTROLLER_SCK_PORT),CONTROLLER_SCK_PIN);

	sbi(DDR(CONTROLLER_DC_PORT),CONTROLLER_DC_PIN);
	sbi(DDR(CONTROLLER_RST_PORT),CONTROLLER_RST_PIN);
	sbi(DDR(CONTROLLER_CS_PORT),CONTROLLER_CS_PIN);

	GLCD_DESELECT();

		/*
	 * Max allowed SPI clock is 4 MHz from datasheet.
	 * Enable SPI, set master mode and clock rate to /4 (4MHz with F_CPU=8MHz)
	 */
	SPCR = (1<<SPE) | (1<<MSTR);// | _BV(SPR0);
	SPSR = 0;

	lcd_reset();

	GLCD_SELECT();

//	commandList(Bcmd);
	// TODO !!! single list !!!
	commandList(Rcmd1);
	commandList(Rcmd2red);
	commandList(Rcmd3);

//	glcd_command(ST7735_MADCTL);
//	glcd_data(0xC0);


	GLCD_DESELECT();

}

static void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	uint8_t rowstart = LCD_DELTA_Y;
	uint8_t colstart = LCD_DELTA_X;

	lcd_command(ST7735_CASET); // Column addr set
  	lcd_data(0x00);
  	lcd_data(x0+colstart);     // XSTART
  	lcd_data(0x00);
  	lcd_data(x1+colstart);     // XEND

	lcd_command(ST7735_RASET); // Row addr set
  	lcd_data(0x00);
  	lcd_data(y0+rowstart);     // YSTART
  	lcd_data(0x00);
  	lcd_data(y1+rowstart);     // YEND

	lcd_command(ST7735_RAMWR); // write to RAM
}

inline static void _setAddrWindowFast(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	uint8_t rowstart = LCD_DELTA_Y;
	uint8_t colstart = LCD_DELTA_X;

	lcd_command(ST7735_CASET); // Column addr set
  	lcd_data(0x00);
  	lcd_data(x0+colstart);     // XSTART
  	lcd_data(0x00);
  	lcd_data(x1+colstart);     // XEND

	lcd_command(ST7735_RASET); // Row addr set
  	lcd_data(0x00);
  	lcd_data(y0+rowstart);     // YSTART
  	lcd_data(0x00);
  	lcd_data(y1+rowstart);     // YEND

	lcd_command(ST7735_RAMWR); // write to RAM
}

void drawPixel(uint8_t x, uint8_t y, uint16_t color) {
//  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
#ifdef SIMULATION
	simulator_draw_pixel(x, y, color);
#else
  setAddrWindow(x, y, x, y+1);
  lcd_data_w(color);
#endif
}

uint8_t _pixel_color_lo;
uint8_t _pixel_color_hi;

inline static void _drawPixelFast(int8_t x, int8_t y) {
//  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

#if SIMULATION
	simulator_draw_pixel(x, y, (_pixel_color_hi << 8) + _pixel_color_lo);
#else
	_setAddrWindowFast(x, y, x, y+1);
	GLCD_DC_HIGH();
	lcd_spi_write_fast(_pixel_color_hi);
	lcd_spi_write_fast(_pixel_color_lo);
#endif
}

void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

void fillScreen(uint16_t color) {
	fillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

// fill a rectangle
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
#if SIMULATION
	//printf("fillRect(%hhu, %hhu, %hhu, %hhu, %hx)\n", x, y, w, h, color);
	simulator_draw_data_start(x, y, w, h);
	uint16_t cnt = w*h;
	while (cnt-- > 0) {
		simulator_draw_next_pixel(color);
	}

#else
//  setAddrWindow(x, y, x+w-1, y+h-1);
  setAddrWindow(x, y, x+w-1, y+h);

  uint8_t hi = color >> 8, lo = color;

	GLCD_DC_HIGH();	// data
	uint16_t cnt = w*h;
	GLCD_SELECT();
	while (cnt-- > 0) {
      lcd_spi_write_fast(hi);
      lcd_spi_write_fast(lo);
}
		GLCD_DESELECT();
#endif

/*
  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      glcd_spi_write(hi);
      glcd_spi_write(lo);
    }
  }
*/
}

void drawDataStart(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
#if SIMULATION
	simulator_draw_data_start(x, y, w, h);
#else
	setAddrWindow(x, y, x+w-1, y+h);
	GLCD_DC_HIGH();	// data
	GLCD_SELECT();
#endif
}

void drawDataFinish() {
	GLCD_DESELECT();
}

void drawDataNext(uint8_t hi, uint8_t lo) {
#if SIMULATION
	simulator_draw_next_pixel((hi << 8) + lo);
#else
	lcd_spi_write_fast(hi);
	lcd_spi_write_fast(lo);
#endif
}

void drawHorizontalLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color) {
#if SIMULATION
	simulator_draw_data_start(x, y, w, 1);
	while (w-- > 0) {
		simulator_draw_next_pixel(color);
	}
#else
	uint8_t hi = color >> 8, lo = color;
	setAddrWindow(x, y, x+w-1, y+1);

	GLCD_DC_HIGH();	// data
	GLCD_SELECT();
	while (w-- > 0) {
      lcd_spi_write_fast(hi);
      lcd_spi_write_fast(lo);
	}
	GLCD_DESELECT();
#endif
}

void drawVerticalLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color) {
#if SIMULATION
	simulator_draw_data_start(x, y, 1, h);
	while (h-- > 0) {
		simulator_draw_next_pixel(color);
	}
#else
	uint8_t hi = color >> 8, lo = color;
	setAddrWindow(x, y, x, y+h);

	GLCD_DC_HIGH();	// data
	GLCD_SELECT();
	while (h-- > 0) {
      lcd_spi_write_fast(hi);
      lcd_spi_write_fast(lo);
	}
	GLCD_DESELECT();
#endif
}



void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color) {
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	uint8_t dx = x1 - x0;
	uint8_t dy = abs(y1 - y0);

	int8_t err = dx / 2;

	int8_t ystep = y0 < y1 ? 1 : -1;

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}


void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
	drawHorizontalLine(x, y,   w, color);
	drawHorizontalLine(x, y+h-1, w, color);
	y++;
	h -= 2;
	drawVerticalLine(x, y, h, color);
	drawVerticalLine(x+w-1, y, h, color);
}

void drawRectThick(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t tx, uint8_t ty, uint16_t color) {
	fillRect(x, y, w, ty, color);
	fillRect(x, y+h-ty, w, ty, color);
	fillRect(x, y+ty, tx, h-ty-ty, color);
	fillRect(x+w-tx, y+ty, tx, h-ty-ty, color);
}



void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color) {
	drawPixel(x0, y0+r, color);
	drawPixel(x0, y0-r, color);
	drawPixel(x0+r, y0, color);
	drawPixel(x0-r, y0, color);

	int8_t x = 0;
	int8_t y = r;
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);

		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}




void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color) {
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;

	drawVerticalLine(x0, y0-r, r + r, color);
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;

		ddF_x += 2;
		f += ddF_x;
		drawVerticalLine(x0-x, y0-y, y + y, color);
		drawVerticalLine(x0+x, y0-y, y + y, color);
		drawVerticalLine(x0-y, y0-x, x + x, color);
		drawVerticalLine(x0+y, y0-x, x + x, color);
	}
}

