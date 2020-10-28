/*
 * File:   general.h
 * Author: trol
 *
 * Created on December 15, 2016, 8:14 PM
 */

#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <stdint.h>

#define sbi(var, mask)   ((var) |= _BV(mask))
#define cbi(var, mask)   ((var) &= ~(_BV(mask)))
#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))


typedef uint8_t (*uint8_func_t)();

#endif // _GENERAL_H_

