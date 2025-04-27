#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "menu.h"
#include "menu.c"

int main() {
    stdio_init_all();
    stdio_flush();

    char buf[MAX_INPUT_LEN];
    millis_delay(2800); // startup delay so first prints are seen on serial terminal
    buf[0] = getchar(); // first getchar always null ?

    printStartup();

    while (true) {
        memset(buf, 0, MAX_INPUT_LEN);
        if (current_menu == mainMenu) {
            printf("hodlr> ");
        } else {
            printf("admin> ");
        }
        readInput(&buf[0], MAX_INPUT_LEN);
        printf("\n"); 

        parseMenu(current_menu, buf);

        sleep_ms(50);
    }
    return 0;
}
