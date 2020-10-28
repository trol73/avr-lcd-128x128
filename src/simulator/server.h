/* 
 * File:   server.h
 * Author: trol
 *
 * Created on March 20, 2017, 22:49 PM
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdint.h>
#include <stdbool.h>

bool server_start();
int server_read_next_byte();
void server_write_byte(int val);
void server_stop();

#endif  // _SERVER_H_

