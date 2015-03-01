/* 
 * File:   display.h
 * Author: Bruno
 *
 * Created on 10 février 2015, 12:45
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#include "font.h"

#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF

#define COLUMNS 80
#define ROWS    30

#define FRAME_SIZE 9600
extern char __attribute__((far,noload)) frame[FRAME_SIZE];
extern volatile char* frame_ptr;
extern volatile char vsync_flag;
extern unsigned char cursor_x, cursor_y;

void display_vsync_cursor_blinking();
void display_reset();
void display_clear();
void display_clear_cursor();
void display_put_map(unsigned char c);
void display_put_char(unsigned char c);
void display_move_cursor_right();
void display_move_cursor_left();
void display_move_cursor_up();
void display_move_cursor_down();
void display_set_cursor(unsigned char x, unsigned char y);
void display_scroll_up();
void display_scroll_down();
void display_show_cursor();
void display_hide_cursor();

#endif	/* DISPLAY_H */

