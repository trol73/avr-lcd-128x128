/* 
 * File:   commands.h
 * Author: trol
 *
 * Created on December 16, 2016, 8:39 PM
 */

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


typedef void (*void_proc_t)();
//typedef void (*uint8_proc_t)(uint8_t);
//typedef uint8_t (*uint8_func_t)();

void CmdInit();
void CmdExec(uint8_t code);


#endif // _COMMANDS_H_

