/* 
 * File:   vt100.h
 * Author: Bruno
 *
 * Created on 10 février 2015, 12:45
 */

#ifndef VT100_H
#define	VT100_H

#include "bool.h"
#include "display.h"

#define ESCAPE  0x1B

#define VT100_HIDE_CURSOR  "25l"
#define VT100_SHOW_CURSOR  "25h"

#define VT100_BUFFER_SIZE  10

extern char vt100_buffer[VT100_BUFFER_SIZE];
extern unsigned char vt100_buffer_count;

char is_alpha(unsigned char c);
char is_numeric(unsigned char c);
char cmp(const char* c1, const char* c2);

void vt100_erase_screen();
void vt100_erase_line();
void vt100_render_attribute();
void vt100_scroll_area();
void vt100_home();
void vt100_save_cursor();
void vt100_restore_cursor();
void vt100_clear_buffer();
void vt100_clear_screen();
void vt100_cursor_home();
void vt100_handler();

#endif	/* VT100_H */

