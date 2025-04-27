#include "config.h"

void millis_delay(uint32_t wait_ms) {
    // "nonblocking" ms delay
    uint32_t prev_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) - prev_time < wait_ms) {
    }
}

void readInput(char *buf, uint16_t len) {
    // read `len` bytes into `*buf` or until newline/null is reached 
    char c;
    uint16_t i=0;

    if (len > MAX_INPUT_LEN) {
        len = MAX_INPUT_LEN;
    }
    while (1) {
        c = getchar();
        if (c != '\n' && c != '\r' && c != '\0' && (i < len-1)) {
            buf[i++] = c;
        } else {
            buf[i] = '\0';
            break;
        }
    }
}
