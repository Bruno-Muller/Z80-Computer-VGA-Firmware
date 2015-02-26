/* 
 * File:   vt100.h
 * Author: Bruno
 *
 * Created on 10 février 2015, 12:45
 */

#ifndef VT100_H
#define	VT100_H

#include "display.h"

#define ESCAPE  0x1B

#define VT100_CURSOR_HOME   "\33H"
#define VT100_CLEAR_SCREEN  "\332J"

#define VT100_BUFFER_SIZE  10
static char vt100_buffer[VT100_BUFFER_SIZE];
static unsigned char vt100_buffer_count = 0;

void vt100_clear_buffer();
void vt100_clear_screen();
void vt100_cursor_home();
void vt100_handler();

#endif	/* VT100_H */

