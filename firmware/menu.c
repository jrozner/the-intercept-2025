
#include "menu.h"

menuItem mainMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"login", "Login to administrator console", ENABLED, adminLogin},
 {"mining", "Mine various sidecoins", ENABLED, miningLogin}, // TODO
 {"TODO2", "flag{test_flag}", DISABLED, NULL}, // TODO
};

menuItem advancedMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"logout", "Logout of admin console; return to main menu", ENABLED, adminLogout},
};

menuItem miningMenu[MAX_MENU_ITEMS] = {
 {"rugcoin", "Mine some RugCoin", ENABLED, mineRugCoin},
 {"inflatocash", "Mine some InflatoCash", ENABLED, mineInflatoCash},
 {"back", "Go back to main menu", ENABLED, miningLogout},
};

menuItem *current_menu = mainMenu;



void adminLogin(void) {

    uint8_t i;
    uint8_t err = 0;

    // flag{adM1n_t1Me_y0}
    char real_password[20] = { 'f','l','a','g','{','a','d','M','1','n','_','t','1','M','e','_','y','0','}','\0'};
    char user_password[20];

    printf("Enter administrative password:\n");
    printf(">>> ");
    // read in user password input
    readInput(user_password, 20);

    if (strlen(user_password) == strlen(real_password)) {
        for (i=0; i<strlen(real_password); i++) {
            if (user_password[i] != real_password[i]) {
                err = 1;
                break;
            } else { // delay per correct character checked
                millis_delay(200);
            }
        }
    } else {
        err = 1;
    }
    if (err) {
        printf("Wrong password detected!\n");
    } else {
        printf("\n");
        printf("ACCESS GRANTED!\n");
        current_menu = advancedMenu;
    } 
}

void adminLogout(void) {
    if (current_menu == advancedMenu) {
        printf("[+] Logging out of admin console . . .\n\n");
        current_menu = mainMenu;
    }
}

void miningLogin(void) {
    current_menu = miningMenu;
}

void miningLogout(void) {
    current_menu = mainMenu;
}

void parseMenu(menuItem *menu, char *cmd_buf) {
    uint8_t i;
    for (i=0; i<MAX_MENU_ITEMS; i++) {
        if (menu[i].name != NULL && menu[i].visible) {
            //printf("Checking %s...\n", menu[i].name); //TODO debug remove
            if (strcmp(menu[i].name, cmd_buf) == 0) {
                //printf("found match\n"); //TODO debug remove
                if (menu[i].func != NULL) {
                    (menu[i].func)();
                } else {
                    printf("[!] Command \"%s\" not implemented\n", cmd_buf);
                }
                return;
            }
        }
    }
    printf("[!] Unknown command: \"%s\"\n", cmd_buf);
}

void printHelp(void) {
    // print visible commands and descriptions for menu
    // uses current_menu ptr
    uint8_t i;
    for (i=0; i<MAX_MENU_ITEMS; i++) {
        if (current_menu[i].name != NULL && current_menu[i].visible) {
            printf("%08s - %s\n", current_menu[i].name, current_menu[i].description); 
        }
    }
}

void printStartup(void) {
    printf("  flag{s3r10us_ab0ut_s3r1al}\n");
    printf("    bagHODLr - Buy the DIP\n");
    printf("      __________________\n");
    printf("    .-'  \\ _.-''-._ /  '-.\n");
    printf("  .-/\\   .'.      .'.   /\\-.\n");
    printf(" _'/  \\.'   '.  .'   './  \\'_\n");
    printf(":======:======::======:======:\n");
    printf(" '. '.  \\     ''     /  .' .'\n");
    printf("   '. .  \\   :  :   /  . .'\n");
    printf("     '.'  \\  '  '  /  '.'\n");
    printf("       ':  \\:    :/  :'\n");
    printf("         '. \\    / .'\n");
    printf("           '.\\  /.'\n");
    printf("    DIAMOND  '\\/'  HANDS\n");
    printf("-------------------------------\n");
    printf("   Secure Wallet Console v.1\n");
    printf("-------------------------------\n");
    printf("\n");
}

