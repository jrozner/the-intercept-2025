#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdlib.h>
#include <string.h>

void printMainHelp(void) {
    uint8_t i;
    for (i=0; i<MAX_MENU_ITEMS; i++) {
        if (mainMenu[i].name != NULL && mainMenu[i].visible) {
            printf("%08s - %s\n", mainMenu[i].name, mainMenu[i].description); 
        }
    }
}

void printStartup(void) {
    printf("  flag{s3r10us_ab0ut_s3r1al}\n");
    printf("    bagHODLr - Buy the DIP\n");
    printf("      __________________               \n");
    printf("    .-'  \\ _.-''-._ /  '-.             \n");
    printf("  .-/\\   .'.      .'.   /\\-.           \n");
    printf(" _'/  \\.'   '.  .'   './  \\'_          \n");
    printf(":======:======::======:======:          \n");
    printf(" '. '.  \\     ''     /  .' .'          \n");
    printf("   '. .  \\   :  :   /  . .'            \n");
    printf("     '.'  \\  '  '  /  '.'              \n");
    printf("       ':  \\:    :/  :'                \n");
    printf("         '. \\    / .'                  \n");
    printf("           '.\\  /.'                    \n");
    printf("    DIAMOND  '\\/'  HANDS\n");
    printf("\n");
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
        if (c != '\n' && c != '\r' && c != '\0' && (i < len-2)) {
            buf[i++] = c;
        } else {
            buf[i] = '\0';
            break;
        }
    }
}
