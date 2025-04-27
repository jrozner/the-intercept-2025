

#include "mining.h"


int generateMath()
{
    int answer = rand() % 99 + 1;

    printf("%d", answer);

    for(int i = 0; i < 10; i++)
    {
        int operand = rand() % 99 + 1;

        int op = rand() % 4;

        if (op == 0) {
            printf("+%d", operand);
            answer += operand;
        } else if (op == 1) {
            printf("-%d", operand);
            answer -= operand;
        } else if (op == 2) {
            printf("*%d", operand);
            answer *= operand;
        } else if (op == 3) {
            printf("/%d", operand);
            answer /= operand;
        }
    }

    printf("\n");
    printf("answer is %d\n", answer);

    return answer;
}

void mineRugCoin(void)
{
    printf("Mining some RugCoin!\n");
    printf("Type quit to return to the menu\n");

    char input[20];

    for(int i = 0; i < 1000; i++)
    {
        int answer = generateMath();
       
        printf("?");

        readInput(input, 20);

        if (strcmp(input, "quit") == 0)
            break;

        if (atoi(input) == answer)
        {
            printf("Correct!\n");
        }
        else
        {
            printf("Incorrect expected %d got %d!\n", answer, atoi(input));
            return;
        }
    }

    printf("You earned one RugCoin!\n");
    printf("Now back to the mines with you!\n");
    printf("flag{th3_ch1ldr3n_y34rn_f0r_th3_m1n3s}\n");
}

typedef struct block_t
{
    uint8_t hash[16];
    struct block_t *next;
    struct block_t *prev;
} Block;

void mineInflatoCash()
{
    const char *msg = "hello world";
    uint8_t hash[16];
    MD5_CTX ctx;

    md5_init(&ctx);
    md5_update(&ctx, msg, strlen(msg));
    md5_final(&ctx, hash);

    printf("MD5 = ");
    for (int i = 0; i < 16; i++)
        printf("%02x", hash[i]);
    printf("\n");       


}





