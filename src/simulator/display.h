#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#if SIMULATION

#include <stdbool.h>

#include "SDL.h"

#include "stubs/avr/io.h"

#include "../config.h"

bool simulator_pool_event();
void render_sdl();
void simulator_draw_pixel(uint8_t x, uint8_t y, uint16_t color);
void simulator_draw_data_start(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void simulator_draw_next_pixel(uint16_t color);
int simulator_get_next_byte();

void simulator_to_front();

#endif // SIMULATION
#endif // _DISPLAY_H_