/*
 * File:   utils.h
 * Author: trol
 *
 * Created on September 6, 2017, 22:26 PM
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include "config.h"
#include "general.h"

#include <stdint.h>
#include <stdbool.h>


uint8_t drawUint16XY(uint8_t x, uint8_t y, uint16_t val, uint16_t color);
uint8_t loadObject(uint8_t size, uint8_t clearFrom, uint8_func_t loadFunc);

#endif // _UTILS_H_

