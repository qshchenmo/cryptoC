/*
 * File       : sha.c
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-10-05     QshLyc       first version
 */

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../base/object.h"
#include "../util/rolate_shift.h"
#include "md5.h"


uint8_t pad[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
//#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
          { \
          a += F(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define GG(a,b,c,d,x,s,ac) \
          { \
          a += G(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define HH(a,b,c,d,x,s,ac) \
          { \
          a += H(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define II(a,b,c,d,x,s,ac) \
          { \
          a += I(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }                                            

static void md5_ctx_init(md5_ctx *ctx)
{
    ctx->count[0] = 0;
    ctx->count[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;

    return;
}

static void md5_encode(uint8_t *output,uint32_t *input,uint32_t len)
{
    uint32_t i = 0,j = 0;
    while(j < len)
    {
        output[j] = input[i] & 0xFF;  
        output[j+1] = (input[i] >> 8) & 0xFF;
        output[j+2] = (input[i] >> 16) & 0xFF;
        output[j+3] = (input[i] >> 24) & 0xFF;
        i++;
        j+=4;
    }
}

static void md5_decode(uint32_t *output,uint8_t *input,uint32_t len)
{
     uint32_t i = 0,j = 0;
     while(j < len)
     {
        output[i] = (input[j]) |
                    (input[j+1] << 8) |
                    (input[j+2] << 16) |
                    (input[j+3] << 24);
        i++;
        j+=4; 
     }
}

static void md5_core(uint32_t state[4],uint8_t block[64])
{
    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t x[64];
    md5_decode(x,block,64);
    FF(a, b, c, d, x[ 0], 7, 0xd76aa478); /* 1 */
 	FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */
 	FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */
 	FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[ 4], 7, 0xf57c0faf); /* 5 */
	FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[ 8], 7, 0x698098d8); /* 9 */
	FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[ 1], 5, 0xf61e2562); /* 17 */
	GG(d, a, b, c, x[ 6], 9, 0xc040b340); /* 18 */
	GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[ 5], 5, 0xd62f105d); /* 21 */
	GG(d, a, b, c, x[10], 9,  0x2441453); /* 22 */
	GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[ 9], 5, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[ 5], 4, 0xfffa3942); /* 33 */
	HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[ 1], 4, 0xa4beea44); /* 37 */
	HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[ 6], 23,  0x4881d05); /* 44 */
	HH(a, b, c, d, x[ 9], 4, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II(a, b, c, d, x[ 0], 6, 0xf4292244); /* 49 */
	II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[ 8], 6, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[ 4], 6, 0xf7537e82); /* 61 */
	II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

static void md5_update(md5_ctx *ctx, uint8_t *input, uint32_t inputlen)
{
    uint32_t i = 0, usedlen = 0,freelen = 0;
    usedlen  = (ctx->count[0] >> 3) & 0x3F;
    freelen =  64 - usedlen;
    
    /* add the count */
    ctx->count[0] += inputlen << 3;
    if(ctx->count[0] < (inputlen << 3)) 
    {
    	/* overflow */
    	ctx->count[1]++;
    }	
       
    ctx->count[1] += inputlen >> 29;
    
    if(inputlen >= freelen)
    {
       memcpy(&ctx->buffer[usedlen], input, freelen);
       md5_core(ctx->state,ctx->buffer);
       for(i = freelen;i+64 <= inputlen;i+=64)
           md5_core(ctx->state,&input[i]);
       usedlen = 0;        
    }  
    else
    {
        i = 0;
    }
    memcpy(&ctx->buffer[usedlen],&input[i],inputlen-i);
}

void md5_final(md5_ctx *ctx, uint8_t digest[16])
{
    uint32_t usedlen = 0,padlen = 0;
    uint8_t bits[8];
    usedlen = (ctx->count[0] >> 3) & 0x3F;
    padlen = (usedlen < 56) ? (56 - usedlen):(120 - usedlen);
    md5_encode(bits,ctx->count,8);
    md5_update(ctx, pad, padlen);
    md5_update(ctx, bits, 8);
    md5_encode(digest, ctx->state, 16);
}

static void* md5_ctor(void *_self, va_list *app)
{
	MD5 *self = _self;
	
	md5_ctx_init(&self->ctx);

	return _self;
}

static void* md5_dtor(void* _self)
{
	MD5* self = _self;
	
	return _self;
}

static const OBJECT _Md5 = {
    sizeof(MD5),
    NULL,
    md5_ctor, 
    md5_dtor,	
};

const void* Md5 = &_Md5;

void MD5_CalculateDigest(void* _self, uint8_t* input, uint32_t inputlen, uint8_t *digest)
{
	MD5 *self = _self;

	md5_update(&self->ctx, input, inputlen);  
    
    md5_final(&self->ctx, digest);          

    return;
}