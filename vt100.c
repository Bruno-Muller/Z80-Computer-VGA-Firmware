#include "vt100.h"

char vt100_buffer[VT100_BUFFER_SIZE];
unsigned char vt100_buffer_count = 0;

unsigned char vt100_saved_cursor_x = 0;
unsigned char vt100_saved_cursor_y = 0;

char is_alpha(unsigned char c) {
    if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) return TRUE;
    else return FALSE;
}

char is_numeric(unsigned char c) {
    if ((c >= '0') && (c <= '9')) return TRUE;
    else return FALSE;
}

char cmp(const char* s1, const char* s2) {
    do {
        if (*s1 != *s2) return FALSE;
        s1++;
        s2++;
    } while ((*s1 != 0) && (*s2 != 0));
    return TRUE;
}

void vt100_handler() {
    if (vt100_buffer_count == 2) {
        if (vt100_buffer[1] == '7') {
            vt100_save_cursor();
            vt100_clear_buffer();
        }
        else if (vt100_buffer[1] == '8') {
            vt100_restore_cursor();
            vt100_clear_buffer();
        }
    }
    else {
        unsigned char last_char = vt100_buffer[vt100_buffer_count-1];
        if (!is_alpha(last_char)) return;

        if (vt100_buffer[3] == '?') {
            if (cmp(&vt100_buffer[4], VT100_HIDE_CURSOR)) display_hide_cursor();
            else if (cmp(&vt100_buffer[4], VT100_SHOW_CURSOR)) display_show_cursor();
        }
        else {
            switch (last_char) {
                case 'H': vt100_home();             break;
                case 'r': vt100_scroll_area();      break;
                case 'm': vt100_render_attribute(); break;
                case 'J': vt100_erase_screen();     break;
                case 'K': vt100_erase_line();       break;
            }
        }
        vt100_clear_buffer();
    }
}

 void vt100_erase_screen() {
    unsigned char p = vt100_buffer[4];
    switch (p) {
        case 0: break;
        case 1: break;
        case 2: display_clear(); break;
    }
 }

void vt100_erase_line() {
    unsigned char p = vt100_buffer[4];
    switch (p) {
        case 0: break;
        case 1: break;
        case 2: break;
    }
}

void vt100_render_attribute() {

}

void vt100_scroll_area() {

}

void vt100_home() {
    unsigned char p1, p2;
    p1 = 0;
    p2 = 0;
    display_set_cursor(p1, p2);
}

void vt100_save_cursor() {
    vt100_saved_cursor_x = cursor_x;
    vt100_saved_cursor_y = cursor_y;
}

void vt100_restore_cursor() {
    cursor_x = vt100_saved_cursor_x;
    cursor_y = vt100_saved_cursor_y;
}

void vt100_clear_buffer() {
    for (vt100_buffer_count=0; vt100_buffer_count<VT100_BUFFER_SIZE; vt100_buffer_count++) vt100_buffer[vt100_buffer_count] = 0;
    vt100_buffer_count = 0;
}