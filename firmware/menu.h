#ifndef MENU_H
#define MENU_H

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

void printStartup(void);
void printHelp(void);

menuItem mainMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"login", "Login to administrator console", ENABLED, adminLogin},
 {"TODO", "Would you like to play a game?", ENABLED, NULL}, // TODO
 {"TODO2", "flag{test_flag}", DISABLED, NULL}, // TODO
};

menuItem advancedMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"logout", "Logout of admin console; return to main menu", ENABLED, adminLogout},
};

menuItem *current_menu = mainMenu;

#endif
