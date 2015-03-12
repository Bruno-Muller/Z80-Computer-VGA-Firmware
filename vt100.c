#include "vt100.h"

typedef struct {
    unsigned char x;
    unsigned char y;
} Cursor;


Cursor vt100_saved_cursor = {0, 0};
Buffer vt100_buffer;

char is_alpha(unsigned char c) {
    if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) return TRUE;
    else return FALSE;
}

char is_numeric(unsigned char c) {
    if ((c >= '0') && (c <= '9')) return TRUE;
    else return FALSE;
}

unsigned char get_param_n(const char* s, unsigned char n) {
    unsigned char i = 0;
    while(i!=n) {
        if (!is_numeric(*s)) i++;
        s++;
    }
    return get_param(s);
}


unsigned char get_param(const char* s) {
    unsigned char p = 0;

    while (is_numeric(*s)) {
        p = (p*10) + (*s - '0');
        s++;
    }

    return p;
}

char compare(const char* s1, const char* s2) {
    do {
        if (*s1 != *s2) return FALSE;
        s1++;
        s2++;
    } while ((*s1 != 0) && (*s2 != 0));
    return TRUE;
}

void vt100_handler() {
    if (vt100_buffer.count == 2) {
        if (vt100_buffer.data[1] == '7') {
            vt100_save_cursor();
            vt100_clear_buffer();
        }
        else if (vt100_buffer.data[1] == '8') {
            vt100_restore_cursor();
            vt100_clear_buffer();
        }
    }
    else {
        unsigned char last_char = vt100_buffer.data[vt100_buffer.count-1];
        if (!is_alpha(last_char)) return;

        if (vt100_buffer.data[2] == '?') {
            if (compare(&vt100_buffer.data[3], VT100_HIDE_CURSOR)) display_hide_cursor();
            else if (compare(&vt100_buffer.data[3], VT100_SHOW_CURSOR)) display_show_cursor();
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
    unsigned char p = vt100_buffer.data[2];
    switch (p) {
        case '0': break;
        case '1': break;
        case '2': display_clear_screen(); break;
    }
 }

void vt100_erase_line() {
    unsigned char p = vt100_buffer.data[2];
    switch (p) {
        case '0': break;
        case '1': break;
        case '2': display_clear_line(); break;
    }
}

void vt100_render_attribute() {
    unsigned char p = get_param_n(&vt100_buffer.data[2], 0);
    switch (p) {
        case 7:
            display_set_background_color(COLOR_WHITE);
            display_set_foreground_color(COLOR_BLACK);
            break;
        case 27:
            display_set_background_color(COLOR_BLACK);
            display_set_foreground_color(COLOR_WHITE);
            break;
        case 30:
            display_set_foreground_color(COLOR_BLACK);
            break;
        case 37:
        case 39:;
            display_set_foreground_color(COLOR_WHITE);
            break;
        case 49:
        case 40:
            display_set_background_color(COLOR_BLACK);
            break;
        case 47:
            display_set_background_color(COLOR_WHITE);
            break;
    }
}

void vt100_scroll_area() {
    unsigned char p0, p1;
    p0 = get_param_n(&vt100_buffer.data[2], 0);
    p1 = get_param_n(&vt100_buffer.data[2], 1);

    if (p0!=0) p0--;
    if (p1!=0) p1--;

    display_set_scroll_area(p0, p1);
    display_set_cursor(p0, 0);
}

void vt100_home() {
    unsigned char p0, p1;
    p0 = get_param_n(&vt100_buffer.data[2], 0);
    p1 = get_param_n(&vt100_buffer.data[2], 1);

    if (p0!=0) p0--;
    if (p1!=0) p1--;
    
    display_set_cursor(p1, p0);
}

void vt100_save_cursor() {
    vt100_saved_cursor.x = cursor_x;
    vt100_saved_cursor.y = cursor_y;
}

void vt100_restore_cursor() {
    cursor_x = vt100_saved_cursor.x;
    cursor_y = vt100_saved_cursor.y;
}

void vt100_clear_buffer() {
    for (vt100_buffer.count=0; vt100_buffer.count<VT100_BUFFER_SIZE; vt100_buffer.count++) vt100_buffer.data[vt100_buffer.count] = 0;
    vt100_buffer.count = 0;
}