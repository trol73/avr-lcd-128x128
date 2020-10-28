/*
 * File:   vgi.h
 * Author: trol
 *
 * Created on September 4, 2017, 11:49 AM
 */

#ifndef _VGI_H_
#define _VGI_H_

#include "config.h"
#include "general.h"

#include <stdint.h>
#include <stdbool.h>

uint8_t loadVgi(uint8_t size, uint8_t clearFrom, uint8_func_t loadFunc);
void drawVgi(uint8_t id, uint8_t x, uint8_t y);

#endif // _VGI_H_

