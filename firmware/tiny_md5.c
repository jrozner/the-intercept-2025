#include "tiny_md5.h"
#include <string.h>

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x ^ y ^ z)
#define I(x,y,z) (y ^ (x | ~z))

#define LEFTROTATE(x,c) (((x) << (c)) | ((x) >> (32 - (c))))

static const uint32_t k[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const uint32_t r[] = {
     7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
     5, 9,14,20, 5, 9,14,20, 5, 9,14,20, 5, 9,14,20,
     4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
     6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
};

void md5_init(MD5_CTX *ctx) {
    ctx->length = 0;
    ctx->h[0] = 0x67452301;
    ctx->h[1] = 0xefcdab89;
    ctx->h[2] = 0x98badcfe;
    ctx->h[3] = 0x10325476;
}

static void process_block(MD5_CTX *ctx, const uint8_t block[64]) {
    uint32_t a, b, c, d, f, g, temp;
    uint32_t w[16];
    
    for (int i = 0; i < 16; i++) {
        w[i] = (uint32_t)block[i*4] |
               ((uint32_t)block[i*4+1] << 8) |
               ((uint32_t)block[i*4+2] << 16) |
               ((uint32_t)block[i*4+3] << 24);
    }

    a = ctx->h[0];
    b = ctx->h[1];
    c = ctx->h[2];
    d = ctx->h[3];

    for (int i = 0; i < 64; i++) {
        if (i < 16) {
            f = F(b,c,d);
            g = i;
        } else if (i < 32) {
            f = G(b,c,d);
            g = (5*i + 1) % 16;
        } else if (i < 48) {
            f = H(b,c,d);
            g = (3*i + 5) % 16;
        } else {
            f = I(b,c,d);
            g = (7*i) % 16;
        }

        temp = d;
        d = c;
        c = b;
        b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
        a = temp;
    }

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
}

void md5_update(MD5_CTX *ctx, const void *data, size_t size) {
    const uint8_t *ptr = (const uint8_t*)data;
    size_t buffer_bytes = ctx->length % 64;
    ctx->length += size;

    if (buffer_bytes) {
        size_t to_copy = 64 - buffer_bytes;
        if (size < to_copy) {
            memcpy(ctx->buffer + buffer_bytes, ptr, size);
            return;
        }
        memcpy(ctx->buffer + buffer_bytes, ptr, to_copy);
        process_block(ctx, ctx->buffer);
        ptr += to_copy;
        size -= to_copy;
    }

    while (size >= 64) {
        process_block(ctx, ptr);
        ptr += 64;
        size -= 64;
    }

    memcpy(ctx->buffer, ptr, size);
}

void md5_final(MD5_CTX *ctx, uint8_t result[16]) {
    uint64_t bitlen = ctx->length * 8;
    size_t buffer_bytes = ctx->length % 64;

    ctx->buffer[buffer_bytes++] = 0x80;
    if (buffer_bytes > 56) {
        while (buffer_bytes < 64)
            ctx->buffer[buffer_bytes++] = 0x00;
        process_block(ctx, ctx->buffer);
        buffer_bytes = 0;
    }
    while (buffer_bytes < 56)
        ctx->buffer[buffer_bytes++] = 0x00;

    for (int i = 0; i < 8; i++)
        ctx->buffer[56 + i] = (bitlen >> (8 * i)) & 0xFF;

    process_block(ctx, ctx->buffer);

    for (int i = 0; i < 4; i++) {
        result[i*4] = ctx->h[i] & 0xFF;
        result[i*4+1] = (ctx->h[i] >> 8) & 0xFF;
        result[i*4+2] = (ctx->h[i] >> 16) & 0xFF;
        result[i*4+3] = (ctx->h[i] >> 24) & 0xFF;
    }
}
