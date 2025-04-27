#ifndef MENU_H
#define MENU_H

#define MAX_MENU_ITEMS (10)

void printMainHelp(void);
void printStartup(void);

void readInput(char *buf, uint16_t len);

typedef struct {
    const char *name;
    const char *description;
    uint8_t visible;
    void (*func)();
} menuItem;

menuItem mainMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printMainHelp},
 {"login", "Login to administrator console", ENABLED, NULL},
};

#endif
