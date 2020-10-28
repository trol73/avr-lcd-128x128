#include "fonts.h"

#include <avr/pgmspace.h>


#include "fonts/font5x7.h"
#include "fonts/tahoma13x15.h"

#include "text.h"


void selectFont(uint8_t font) {
	switch (font) {
		case FONT_5x7:
			setFont(Font5x7, 5, 7, 32, 0xff, STANG);
			break;
		case FONT_TAHOMA_13x15:
			setFont(Tahoma13x15, 13, 15, 32, 0xff, MIKRO);
			break;
	}
}
