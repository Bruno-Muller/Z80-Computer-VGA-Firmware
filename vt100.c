#include "vt100.h"

void vt100_handler() {
    if (vt100_buffer[2] == 'H') {
        vt100_cursor_home();
    } else if ((vt100_buffer[2] == '2') && (vt100_buffer[3] == 'J')) {
         vt100_clear_screen();
    }
}

void vt100_clear_buffer() {
    for (vt100_buffer_count=0; vt100_buffer_count<VT100_BUFFER_SIZE; vt100_buffer_count++)
        vt100_buffer[vt100_buffer_count] = 0;
    vt100_buffer_count = 0;
}

void vt100_cursor_home() {
    display_set_cursor(0, 0);
    vt100_clear_buffer();
}

void vt100_clear_screen() {
    display_clear();
    vt100_clear_buffer();
}