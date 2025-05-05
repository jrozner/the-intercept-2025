
#include "chain.h"
#include "mining.h"

void printHex(uint8_t *data, int len)
{
    for(int i = 0; i < len; i++)
        printf("%02x", data[i]);
    printf("\n");
}

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
    myWallet[0].amount += 1;
}

typedef struct block_t
{
    uint8_t prevHash[16];
    uint8_t nonce[16];
    uint8_t publicKey[16];
    uint8_t thisHash[16];
    struct block_t *prev;
} Block;

void makeHash(uint8_t *hashOut, uint8_t *data, size_t len)
{
    MD5_CTX ctx;

    md5_init(&ctx);
    md5_update(&ctx, data, len);
    md5_final(&ctx, hashOut);

}

void hashBlock(Block *block)
{
    //makeHash(&block->thisHash, block, sizeof(block) - sizeof(block->thisHash) - sizeof(block->prev));
}

void initMining()
{
    Block *blocks = (Block *) chain;

    Block *p = (Block *) lastBlock;

    for(int i = 0; i < numBlocks - 1; i++)
        blocks[i+1].prev = &blocks[i];

    while(p)
    {
        printf("block\n");
        p = p->prev;
    }
}

// Convert one hex digit ('0'-'9', 'a'-'f', 'A'-'F') to 0-15
uint8_t hexCharToNibble(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    return 0; // or handle error
}

// Convert hex string to binary buffer
// hexStr must be even length
void hexStringToBytes(const char *hexStr, uint8_t *outBuf, size_t outBufSize) {
    for (size_t i = 0; i < outBufSize; i++) {
        uint8_t highNibble = hexCharToNibble(hexStr[i*2]);
        uint8_t lowNibble  = hexCharToNibble(hexStr[i*2 + 1]);
        outBuf[i] = (highNibble << 4) | lowNibble;
    }
}

int inputHex(uint8_t *dest, int len)
{
    memset(dest, '\0', len * 2);
    
    char tmpBuf[len * 2 + 1];
    memset(tmpBuf, '\0', len * 2 + 1);
    
    readInput(tmpBuf, len * 2 + 1);

    hexStringToBytes(tmpBuf, dest, len);
}

void checkChain(void)
{
    myWallet[4].amount = 0;

    uint8_t * p = lastBlock;

    while(1)
    {
        Block *b = (Block *) p;


        if (memcmp(myWallet[4].pubkey, b->publicKey, 16) == 0)
        {
     
            char tmpHash[16];
            makeHash(tmpHash, (uint8_t *)b, 16 * 3);

            if (memcmp(&b->thisHash, &tmpHash, 16) == 0)
            {
                myWallet[4].amount += 1;
            }
        }

        uint32_t prev = (uint32_t) b->prev;

        if (prev == 0)
        {
            // root of chain
            break;
        }
        else if ((prev >= (uint32_t)chain) && (prev + sizeof(Block) < (uint32_t)chain + sizeof(Block) * maxBlocks))
        {
            // valid address
            
            p = (uint8_t *) prev;
        }
        else
        {
            // invalid address
            break;
        }
    }

    if (myWallet[4].amount >= 20)
    {
        char enc_key[] = "\x24\x3b\x19\xfa\x59\x26\x89\x28\xd6\x42\xc0\x80\x7a\x12\xcf\x28\x3d\x93\xe5\x7a\x05\xf6\xa9\x6f\x41\x03\x69\x3f\xa7\x0d\x5c\xce\xea\x4c\xf3\xf2\x91\xc5";
        char enc_flag[] = "\x42\x57\x78\x9d\x22\x48\xe8\x60\x89\x20\xb2\xb0\x25\x66\xa7\x1c\x49\xcc\x92\x4e\x76\xa9\xc3\x1a\x74\x77\x36\x4b\xcf\x3e\x03\xbd\x84\x7f\xc0\x88\xf4\xb8";

        printf("Congrats, you have at least 20 inflatoCash\nYou are the supreme bagholder!\n");
        
        for(int i = 0; i < 38; i++)
            printf("%c", enc_key[i] ^ enc_flag[i]);

        printf("\n");
    }
}





void inflatoAppend(void)
{
    Block *last = (Block *) lastBlock;
   
    Block *new = last + 1;
    memset((uint8_t*) new, '\0', sizeof(Block));

    printf("Adding newly mined block:\n");
    printf("all inputs should be in hex\n");
    printf("ex: a10f34ff04c1\n");

    printf("  Hash of previous block: ");
    inputHex((uint8_t*) &new->prevHash, 16);
    

    printf("  Nonce: ");
    inputHex((uint8_t*)&new->nonce, 16);
  
    printf("  Public key of miner: ");
    inputHex((uint8_t*)&new->publicKey, 16);
    
    if (memcmp(&new->prevHash, &last->thisHash, 16) != 0)
    {
        memset((uint8_t*) new, '\0', sizeof(Block));
        printf("  bad hash of previous block!\n");
        return;
    }
        
    printf("  Hash of this block: ");
    inputHex((uint8_t*)&new->thisHash, 16);
   
    char tmpHash[16];
    makeHash(tmpHash, (uint8_t *)new, 16 * 3);

    if (memcmp(&new->thisHash, &tmpHash, 16) != 0)
    {
        memset((uint8_t*) new, '\0', sizeof(Block));
        printHex(tmpHash, 16);
        printf("  data of this block doesn't match hash\n");
        return;
    }

    if ((tmpHash[15] != 0x00) || (tmpHash[14] != 0x00))
    {
        memset((uint8_t*) new, '\0', sizeof(Block));
        printf("  hash does not meet proof-of-work requirement\n");
        return;
    }

    new->prev = (struct block_t *) lastBlock;

    lastBlock = (uint8_t*) new;

    checkChain();
}

void inflatoShow(void)
{

    Block *last = (Block *) lastBlock;

    printf("Last block in chain:\n");
    printf("  Address: 0x%p\n", last);
    printf("  Hash of previous block: ");
    printHex(last->prevHash, 16);
    printf("  Nonce: ");
    printHex(last->nonce, 16);
    printf("  Public key of miner: ");
    printHex(last->publicKey, 16);
    printf("  Hash of this block: ");
    printHex(last->thisHash, 16);
    printf("  Address of previous block: 0x%p\n", last->prev);
    printf("\n");

    /*
    const char *msg = "hello world";
    uint8_t hash[16];

    makeHash(hash, (uint8_t*) msg, strlen(msg));

    printf("MD5 = ");
    for (int i = 0; i < 16; i++)
        printf("%02x", hash[i]);
    printf("\n");       
    */

}

void inflatoDump(void)
{
    uint8_t * p = lastBlock;

    while(1)
    {
        printf("0x%08p:  ", p);
        printHex(p, sizeof(Block));
        Block *b = (Block *) p;
        uint32_t prev = (uint32_t) b->prev;

        if (prev == 0)
        {
            // root of chain
            break;
        }
        else if ((prev >= (uint32_t)chain) && (prev + sizeof(Block) < (uint32_t)chain + sizeof(Block) * maxBlocks))
        {
            // valid address
            
            p = (uint8_t *) prev;
        }
        else
        {
            // invalid address
            break;
        }
    }
}




