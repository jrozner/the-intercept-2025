#include "config.h"
#include "menu.h"
#include "otp_helpers.h"

int main() {
    stdio_init_all();
    stdio_flush();

    if(!is_locked()) {
        lock_otp_secret();
    }

    lock_sw_lock_48();

    char buf[MAX_INPUT_LEN];
    millis_delay(2800); // startup delay so first prints are seen on serial terminal
    buf[0] = getchar(); // first getchar always null ?

    printStartup();
    walletInit();

    while (true) {
        memset(buf, 0, MAX_INPUT_LEN);
        if (current_menu == miningMenu) {
            printf("miner> ");
        } else if (current_menu == advancedMenu) {
            printf("admin> ");
        } else { // default
            printf("hodlr> ");
        }
        readInput(&buf[0], MAX_INPUT_LEN);
        printf("\n"); 

        parseMenu(current_menu, buf);

        sleep_ms(50);
    }
    return 0;
}
