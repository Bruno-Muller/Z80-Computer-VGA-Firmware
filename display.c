#include "display.h"

unsigned char cursor_x, cursor_y, foreground_color, background_color;

char __attribute__((far,noload)) frame[FRAME_SIZE];
volatile char* frame_ptr;
volatile char vsync_flag;

void display_vsync_cursor_blinking() {
    static int vsync_counter;
    if (vsync_flag == 0) {
        vsync_flag = 0xFF;

        if (++vsync_counter == 60) vsync_counter = 0;

        if (vsync_counter == 0) display_put_map(127);
        else if (vsync_counter == 30) display_put_map(' ');
    }
}

void display_set_cursor(unsigned char x, unsigned char y) {
    display_clear_cursor();
    cursor_x = x;
    cursor_y = y;
}

void display_reset() {
    display_clear();

    vsync_flag = 0xFF;
    
    cursor_x = 0;
    cursor_y = 0;

    foreground_color = COLOR_WHITE;
    background_color = COLOR_BLACK;
}

void display_clear() {
    unsigned int i;
    for(i = 0; i<FRAME_SIZE; i++) {
        frame[i] = COLOR_BLACK;
    }
}

void display_put_map(unsigned char c) {
    unsigned char row = cursor_y * 8;
    unsigned char column = cursor_x / 2;
    unsigned int pos = row  * 40 + column;
    unsigned char erase_mask;

    const unsigned int* font_ptr = &font[c*2];

    int d1 = *font_ptr;
    font_ptr++;
    int d2 = *font_ptr;

    unsigned char r1 = (d1>>12) & 0x0F;
    unsigned char r2 = (d1>>8) & 0x0F;
    unsigned char r3 = (d1>>4) & 0x0F;
    unsigned char r4 = (d1>>0) & 0x0F;
    unsigned char r5 = (d2>>12) & 0x0F;
    unsigned char r6 = (d2>>8) & 0x0F;
    unsigned char r7 = (d2>>4) & 0x0F;
    unsigned char r8 = (d2>>0) & 0x0F;

    // odd = LSB
    // even = MSB
    if ((cursor_x & 0x01) == 0) {
        r1<<=4;
        r2<<=4;
        r3<<=4;
        r4<<=4;
        r5<<=4;
        r6<<=4;
        r7<<=4;
        r8<<=4;
        erase_mask = 0x0F;
    }
    else {
        erase_mask = 0xF0;
    }

    frame[pos] = (frame[pos]&erase_mask) | r1;
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask) | r2;
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask) | r3;
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask) | r4;
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask) | r5;
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask) | r6;
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask) | r7;
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask) | r8;


}

void display_clear_cursor() {
    unsigned char row = cursor_y * 8;
    unsigned char column = cursor_x / 2;
    unsigned int pos = row  * 40 + column;
    unsigned char erase_mask;

    // odd = LSB
    // even = MSB
    if ((cursor_x & 0x01) == 0) {
        erase_mask = 0x0F;
    }
    else {
        erase_mask = 0xF0;
    }

    frame[pos] = (frame[pos]&erase_mask);
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask);
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask);
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask);
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask);
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask);
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask);
    pos+=40;
    frame[pos] =  (frame[pos]&erase_mask);
}

void display_put_char(unsigned char c) {
    if (c == 0x0A) {
        display_clear_cursor();
        cursor_x = 0;
    }
    else if (c == 0x0D) {
        display_clear_cursor();
        display_move_cursor_down();
    }
    else {
        display_put_map(c);
        display_move_cursor_right();
    }
}

void display_move_cursor_right() {
    if (++cursor_x == COLUMNS) {
        cursor_x = 0;
        display_move_cursor_down();
    }
}

void display_move_cursor_left() {
    if (--cursor_x == 0) {
        cursor_x = COLUMNS - 1;
        display_move_cursor_up();
    }
}

void display_move_cursor_up() {
    if (cursor_y > 0) {
        cursor_y--;
    }
}

void display_move_cursor_down() {
    if (cursor_y < (ROWS-1)) {
        cursor_y++;
    }
    else {
        display_scroll_down();
    }
}

void display_scroll_up() {

}

void display_scroll_down() {
    unsigned int i;
    for(i = 0; i<FRAME_SIZE-320; i++) {
        frame[i] = frame[i+320];
    }
    for(i = FRAME_SIZE-320; i<FRAME_SIZE; i++) {
        frame[i] =  COLOR_BLACK;
    }
}

void display_show_cursor() {

}

void display_hide_cursor() {

}