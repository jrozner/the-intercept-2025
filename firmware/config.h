#ifndef CONFIG_H
#define CONFIG_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#define MAX_INPUT_LEN (100)
#define ENABLED (1)
#define DISABLED (0)
#define MAX_MENU_ITEMS (10)

void readInput(char *buf, uint16_t len);

void millis_delay(uint32_t wait_ms);

#endif
