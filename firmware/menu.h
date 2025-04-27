#ifndef MENU_H
#define MENU_H

#include "mining.h"
#include "config.h"

#define MAX_MENU_ITEMS (10)

typedef struct {
    const char *name;
    const char *description;
    uint8_t visible;
    void (*func)();
} menuItem;

void readInput(char *buf, uint16_t len);
void parseMenu(menuItem *menu, char *cmd_buf);

void adminLogin(void);
void adminLogout(void);

void miningLogin(void);
void miningLogout(void);
void mineRugCoin(void);

void printStartup(void);
void printHelp(void);

extern menuItem mainMenu[MAX_MENU_ITEMS];
extern menuItem advancedMenu[MAX_MENU_ITEMS];
extern menuItem miningMenu[MAX_MENU_ITEMS];
extern menuItem *current_menu;

#endif
