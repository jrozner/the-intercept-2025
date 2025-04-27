
#include "mining.h"

void mineRugCoin(void)
{
    printf("Mining some RugCoin!\n");
    printf("Type quit to return to the menu\n");

    char input[20];

    while(1)
    {
        readInput(&input, 20);

        if (strcmp(input, "quit") == 0)
            break;
    }
        
}

