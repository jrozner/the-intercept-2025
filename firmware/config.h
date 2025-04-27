#ifndef CONFIG_H
#define CONFIG_H

#define MAX_INPUT_LEN (100)
#define ENABLED (1)
#define DISABLED (0)
#define MAX_MENU_ITEMS (10)

void millis_delay(uint32_t wait_ms) {
    // "nonblocking" ms delay
    uint32_t prev_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) - prev_time < wait_ms) {
    }
}

#endif
