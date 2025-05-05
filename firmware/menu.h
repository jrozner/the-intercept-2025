#ifndef MENU_H
#define MENU_H

#include "mining.h"
#include "config.h"

#define MAX_MENU_ITEMS (10)
#define MAX_WALLETS (10)
#define MAX_KEY_LEN (129)

typedef struct {
    const char *name;
    const char *description;
    uint8_t visible;
    void (*func)();
} menuItem;

typedef struct {
    char name[16];     // name of coin
    char privkey[MAX_KEY_LEN]; // private key
    char pubkey[MAX_KEY_LEN];  // wallet address
    uint16_t amount;   // amount of coins
} cryptoCoin;

void readInput(char *buf, uint16_t len);
void parseMenu(menuItem *menu, char *cmd_buf);

void walletInit(void);
void coinNewKey(char *key_addr);
void coinInit(uint8_t wallet_idx, char *name);
void coinTransfer(cryptoCoin *src, cryptoCoin *dest, uint16_t amount);
void adminResetPub(void);
void adminShowPub(void);
void adminShowWallets(void);

void adminLogin(void);
void adminLogout(void);

void miningLogin(void);
void miningLogout(void);
void mineRugCoin(void);
void inflatoLogin(void);
void inflatoLogout(void);

void printStartup(void);
void printHelp(void);

extern menuItem mainMenu[MAX_MENU_ITEMS];
extern menuItem advancedMenu[MAX_MENU_ITEMS];
extern menuItem miningMenu[MAX_MENU_ITEMS];
extern menuItem inflatoMenu[MAX_MENU_ITEMS];
extern menuItem *current_menu;

extern cryptoCoin myWallet[MAX_WALLETS];

#endif
