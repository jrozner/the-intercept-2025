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
    millis_delay(3000); // startup delay so first prints are seen on serial terminal
    buf[0] = getchar(); // first getchar always null ?

    printStartup();

    while (true) {
        memset(buf, 0, MAX_INPUT_LEN);
        printf(">>> ");
        readInput(&buf[0], MAX_INPUT_LEN);

        uint8_t i;
        for (i=0; i<MAX_MENU_ITEMS; i++) {
            if (mainMenu[i].name != NULL && mainMenu[i].visible) {
                printf("Checking %s...\n", mainMenu[i].name);
                if (strcmp(mainMenu[i].name, buf) == 0) {
                    printf("found match\n");
                    if (mainMenu[i].func != NULL) {
                        (mainMenu[i].func)();
                        break; // parsed command - can exit early
                    } else {
                        printf("[!] Command not implemented\n");
                        break; // parsed command - can exit early
                    }
                }
            } 
        }

        sleep_ms(50);
    }
    return 0;
}
