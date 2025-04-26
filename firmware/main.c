#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN (100)

void read_input(char *buf, uint16_t len) {
    char c;
    uint16_t i=0;

    if (len > MAX_INPUT_LEN) {
        len = MAX_INPUT_LEN;
    }
    while (1) {
        c = getchar();
        if (c != '\n' && c != '\r' && c != '\0' && (i < len-2)) {
            buf[i++] = c;
        } else {
            buf[i] = '\0';
            break;
        }
    }
}

void millis_delay(uint32_t wait_ms) {
    // "nonblocking" ms delay
    uint32_t prev_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) - prev_time < wait_ms) {
    }
}

int main() {
    stdio_init_all();
    stdio_flush();

    char buf[MAX_INPUT_LEN];
    millis_delay(3000);
    char dummy = getchar(); // first getchar always null ?

    printf("HELLO WORLD STARTUP0\n");
    printf("HELLO WORLD STARTUP1\n");
    printf("HELLO WORLD STARTUP2\n");
    printf("HELLO WORLD STARTUP3\n");
    printf("HELLO WORLD STARTUP4\n");
    printf("HELLO WORLD STARTUP5\n");

    while (true) {
        memset(buf, 0, MAX_INPUT_LEN);
        printf(">>> ");
        read_input(&buf[0], MAX_INPUT_LEN);
        printf("-cmd \"%s\"\n", buf);
        sleep_ms(50);
    }
    return 0;
}
