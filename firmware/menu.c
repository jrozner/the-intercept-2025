
#include "menu.h"

menuItem mainMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"login", "Login to administrator console", ENABLED, adminLogin},
 {"pub_show", "Show a public key for a given wallet", ENABLED, adminShowPub},
 {"pub_reset", "Reset a public key for a given wallet", ENABLED, adminResetPub},
 {"balance", "Show wallet balances", ENABLED, adminShowWallets},
 {"mining", "Mine various sidecoins", ENABLED, miningLogin}, // TODO
 {"TODO2", "flag{test_flag}", DISABLED, NULL}, // TODO
};

menuItem advancedMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"logout", "Logout of admin console; return to main menu", ENABLED, adminLogout},
};

menuItem miningMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"rugcoin", "Mine some RugCoin", ENABLED, mineRugCoin},
 {"inflatocash", "Mine some InflatoCash", ENABLED, inflatoLogin},
 {"back", "Go back to main menu", ENABLED, miningLogout},
};

menuItem inflatoMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"dump", "Show entire chain", ENABLED, inflatoDump},
 {"append", "Add a valid mined block", ENABLED, inflatoAppend},
 {"back", "Go back to main menu", ENABLED, inflatoLogout},
};

menuItem *current_menu = mainMenu;
cryptoCoin myWallet[MAX_WALLETS] = {};

void walletInit(void) {
    coinInit(0, "rugCoin");
    coinInit(1, "xferCoin");
    coinInit(2, "xferCoin");
    coinInit(3, "flagCoin");
    // set flag for "flagCoin" - do not remove
    memset(myWallet[3].privkey, 0, MAX_KEY_LEN);
    strcpy(myWallet[3].privkey, "flag{lol_a_fl4g_by_anY_0ther_n4m3}\0");
    coinInit(4, "inflatoCash");
    memset(myWallet[4].privkey, 0, MAX_KEY_LEN);
    memset(myWallet[4].pubkey, 0, MAX_KEY_LEN);
    strcpy(myWallet[4].privkey, "UMVHSYQDPZBJWXKT\0");
    strcpy(myWallet[4].pubkey,  "XQPTABZNRLCYWKJE\0");


    initMining();
}

void coinNewKey(char *key_addr) {
    uint16_t i;
    for (i=0; i<MAX_KEY_LEN-1; i++) {
        key_addr[i] = 65 + (rand()%26);
    }
    key_addr[MAX_KEY_LEN-1] = '\0';
}

void coinInit(uint8_t wallet_idx, char *name) {
    // add new wallet
    strcpy(myWallet[wallet_idx].name, name);
    coinNewKey(myWallet[wallet_idx].privkey);
    coinNewKey(myWallet[wallet_idx].pubkey);
    myWallet[wallet_idx].amount = 10;
}

void coinTransfer(cryptoCoin *src, cryptoCoin *dest, uint16_t amount) {
    // move coins from one wallet to another of the same type
}

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

void adminResetPub(void) {
    printf("Please enter wallet index to reset PUBKEY:\n");
    printf(">>> ");
    char buf[10];
    readInput(buf, 5);
    uint8_t wallet_idx = atoi(buf);
    if (wallet_idx >= MAX_WALLETS) {
        printf("[!] Invalid wallet index entered...aborting\n");
    }

    // regenerate a public key
    if (strlen(myWallet[wallet_idx].name) != 0) {
        coinNewKey(myWallet[wallet_idx].pubkey);
        printf("Wallet slot %02u pubkey reset, now:\n  %.30s[...]\n", wallet_idx, myWallet[wallet_idx].pubkey);
    } else {
        printf("[!] Can't reset a pubkey for an empty wallet slot!\n");
    }
}

void adminShowPub(void) {
    printf("Please enter wallet index to show PUBKEY:\n");
    printf(">>> ");
    char buf[10];
    readInput(buf, 5);
    uint8_t wallet_idx = atoi(buf);
    if (wallet_idx >= MAX_WALLETS) {
        printf("[!] Invalid wallet index entered...aborting\n");
    }
    // show a public key
    if (strlen(myWallet[wallet_idx].name) != 0) {
        printf("Wallet slot %02u pubkey:\n", wallet_idx);
        uint8_t i;
        for (i=0; i<(MAX_KEY_LEN/32); i++) {
            printf("  %.32s\n", (myWallet[wallet_idx].pubkey)+(i*32));
        }
    } else {
        printf("[!] Can't show pubkey for an empty wallet slot!\n");
    }
}

void adminShowWallets(void) {
    // show details of all wallets with priv key redacted
    printf("Current wallet balances:\n\n");
    printf("  ## | NAME       | NUM | PUBKEY | PRIVKEY\n");
    uint8_t i;
    for (i=0; i<MAX_WALLETS; i++) {
        if (strlen(myWallet[i].name) != 0) {
            printf("  %02u: %08s | %d | Pub: %.8s[...] | Prv: %.8s[...]\n", i, myWallet[i].name, myWallet[i].amount, myWallet[i].pubkey, myWallet[i].privkey);
        } else {
            printf("  %02u: NULL\n", i);
        }
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

void inflatoLogin(void) {
    current_menu = inflatoMenu;
}

void inflatoLogout(void) {
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
    if (strlen(cmd_buf) == 0) {
        //pass
    } else {
        printf("[!] Unknown command: \"%s\"\n", cmd_buf);
        printHelp();
    }
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

