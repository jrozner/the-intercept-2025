
#include "menu.h"

menuItem mainMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"login", "Login to administrator console", ENABLED, adminLogin},
 {"pub_show", "Show a public key for a given wallet", ENABLED, adminShowPub},
 {"balance", "Show wallet balances", ENABLED, adminShowWallets},
 {"mining", "Mine various sidecoins", ENABLED, miningLogin},
 {"about", "bagHODLr credits", ENABLED, printAbout},
 {"secret", "flag{y0u_c4nt_s33_m3}", DISABLED, NULL},
};

menuItem advancedMenu[MAX_MENU_ITEMS] = {
 {"help", "Prints menu help message", ENABLED, printHelp},
 {"priv_show", "Show a private key for a given wallet", ENABLED, adminShowPriv},
 {"priv_reset", "Reset a private key for a given wallet", ENABLED, adminResetPriv},
 {"pub_show", "Show a public key for a given wallet", ENABLED, adminShowPub},
 {"pub_reset", "Reset a public key for a given wallet", ENABLED, adminResetPub},
 {"name_reset", "Reset a name for a given wallet", ENABLED, adminResetName},
 {"balance", "Show wallet balances", ENABLED, adminShowWallets},
 {"back", "Go back to main menu", ENABLED, adminLogout},
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
        return;
    }

    // regenerate a public key
    if (strlen(myWallet[wallet_idx].name) != 0) {
        coinNewKey(myWallet[wallet_idx].pubkey);
        printf("Wallet slot %02u pubkey reset, now:\n  %.30s[...]\n", wallet_idx, myWallet[wallet_idx].pubkey);
    } else {
        printf("[!] Can't reset a pubkey for an empty wallet slot!\n");
    }
}

void adminResetName(void) {
    printf("Please enter wallet index to reset NAME:\n");
    printf(">>> ");
    char buf[10];
    readInput(buf, 5);
    uint8_t wallet_idx = atoi(buf);
    if (wallet_idx >= MAX_WALLETS) {
        printf("[!] Invalid wallet index entered...aborting\n");
        return;
    }
    if (strlen(myWallet[wallet_idx].name) == 0) {
        printf("[!] Can't reset a privkey for an empty wallet slot!\n");
        return;
    }

    printf("Please enter a new name for wallet %02u (%s):\n", wallet_idx, myWallet[wallet_idx].name);
    printf(">>> ");
    char name_buf[17];
    memset(name_buf, 0, 17);
    readInput(name_buf, 17);

    memcpy(myWallet[wallet_idx].name, name_buf, 16);
    printf("[+] Changed wallet %02u name to %s!\n", wallet_idx, name_buf);
}

void adminResetPriv(void) {
    printf("Please enter wallet index to reset PRIVKEY:\n");
    printf(">>> ");
    char buf[10];
    readInput(buf, 5);
    uint8_t wallet_idx = atoi(buf);
    if (wallet_idx >= MAX_WALLETS) {
        printf("[!] Invalid wallet index entered...aborting\n");
        return;
    }
    if (wallet_idx == 3) { // flag coin
        printNah();
        return;
    }
    // regenerate a private key
    if (strlen(myWallet[wallet_idx].name) != 0) {
        coinNewKey(myWallet[wallet_idx].privkey);
        printf("Wallet slot %02u privkey reset, now:\n  %.30s[...]\n", wallet_idx, myWallet[wallet_idx].privkey);
    } else {
        printf("[!] Can't reset a privkey for an empty wallet slot!\n");
    }
}

void adminShowPriv(void) {
    printf("Please enter wallet index to show PRIVKEY:\n");
    printf(">>> ");
    char buf[10];
    readInput(buf, 5);
    uint8_t wallet_idx = atoi(buf);
    if (wallet_idx >= MAX_WALLETS) {
        printf("[!] Invalid wallet index entered...aborting\n");
        return;
    }
    if (wallet_idx == 3) { // flag coin
        printNah();
        return;
    }
    // show a private key
    if (strlen(myWallet[wallet_idx].name) != 0) {
        printf("Wallet slot %02u (%s) privkey:\n", wallet_idx, myWallet[wallet_idx].name);
        uint8_t i;
        for (i=0; i<(MAX_KEY_LEN/32); i++) {
            printf("  %.32s\n", (myWallet[wallet_idx].privkey)+(i*32));
        }
    } else {
        printf("[!] Can't show privkey for an empty wallet slot!\n");
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
        return;
    }
    // show a public key
    if (strlen(myWallet[wallet_idx].name) != 0) {
        printf("Wallet slot %02u (%s) pubkey:\n", wallet_idx, myWallet[wallet_idx].name);
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

void printAbout(void) {
    printf("DERPY            _.u[[/;:,.         .odMMMMMM'\n");
    printf("  CORP        .o888UU[[[/;:-.  .o@P^    MMM^\n");
    printf("             oN88888UU[[[/;::-.        dP^\n");
    printf(" TO         dNMMNN888UU[[[/;:--.   .o@P^\n");
    printf(" THE       ,MMMMMMN888UU[[/;::-. o@^\n");
//    printf(" MOON      f\x08Nl\x08Na\x08Mg\x08M{\x08Mn\x08N0\x08Nw\x088_\x088y\x0880\x08Uu\x08U_\x08[s\x08[3\x08[3\x08/_\x08~m\x08.3\x08o}\x08@\n");
// C don't like that shit
    printf("\x20\x4d\x4f\x4f\x4e\x20\x20\x20\x20\x20\x20\x66\x08\x4e\x6c\x08\x4e\x61\x08\x4d\x67\x08\x4d\x7b\x08\x4d\x6e\x08\x4e\x30\x08\x4e\x77\x08\x38\x5f\x08\x38\x79\x08\x38\x30\x08\x55\x75\x08\x55\x5f\x08\x5b\x73\x08\x5b\x33\x08\x5b\x33\x08\x2f\x5f\x08\x7e\x6d\x08\x2e\x33\x08\x6f\x7d\x08\x40\x0a");
    printf(" AND       888888888UU[[[/o@^-..\n");
    printf(" BEYOND   oI8888UU[[[/o@P^:--..\n");
    printf("       .@^  YUU[[[/o@^;::---..\n");
    printf("     oMP     ^/o@P^;:::---..\n");
    printf("  .dMMM    .o@^ ^;::---...\n");
    printf(" dMMMMMMM@^`       `^^^^\n");
    printf("YMMMUP^                                PROC\n");
    printf(" ^^                                     YPRED\n");
    printf("\n");
    printf("- - - - DerpyCorp - - - -\n");
    printf("Since 2019, the proud folks of DerpyCorp have\n");
    printf("lived up to our company slogan:\n");
    printf("\"If it's not broken, we'll update it until it is!\"\n");
    printf("- - - bagHODLr v0.1 - - -\n");
    printf("buildID 210e5d2e7986d7b6\n");
    printf("Lead Developers:\n");
    printf(" JK\n");
    printf(" datagram\n");
    printf(" Joe Rozner\n");
}

void printNah(void) {
    printf("              ,\n");
    printf("     __  _.-\"` `'-.\n");
    printf("    /||\\'._ __{}_(\n");
    printf("    ||||  |'--.__\\\n");
    printf("    |  L.(   ^_\\^   NAH\n");
    printf("    \\ .-' |   _ |      NAH\n");
    printf("    | |   )\\___/          NAH\n");
    printf("    |  \\-'`:._]\n");
    printf("    \\__/;      '-.\n");
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

