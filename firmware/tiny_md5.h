#ifndef TINY_MD5_H
#define TINY_MD5_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t h[4];
    uint8_t buffer[64];
    uint64_t length;
} MD5_CTX;

void md5_init(MD5_CTX *ctx);
void md5_update(MD5_CTX *ctx, const void *data, size_t size);
void md5_final(MD5_CTX *ctx, uint8_t result[16]);

#endif
