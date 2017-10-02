/*
 * File       : descipher.h
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-08-15     QshLyc       first version
 */

#ifndef _BLOCKCIPHER_H_
#define _BLOCKCIPHER_H_

extern const void* BlockCipher;

#define BLOCKCIPHER_BLOCKSIZE_DEFAULT   8

enum blockcipher_dir_e{
	BLOCKCIPHER_DIR_ENC = 0,
	BLOCKCIPHER_DIR_DEC,
};

enum blockcipher_mode_e{
	BLOCKCIPHER_MODE_ECB = 0,
	BLOCKCIPHER_MODE_CBC,
};

struct blockcipher_mode_operations{
	int (*enc)(void *_self, const cc_uint8_t* plaintext, cc_uint32_t plainlen, cc_uint8_t* ciphertext, cc_uint32_t* cipherlen);
	int (*dec)(void *_self, const cc_uint8_t* ciphertext, cc_uint32_t cipherlen, cc_uint8_t* plaintext, cc_uint32_t* plainlen);
};

enum blockcipher_pad_e{
	BLOCKCIPHER_PAD_ZERO = 0,
	BLOCKCIPHER_PAD_PKCS7,
};

struct blockcipher_pad_operations{
	int (*pad)(void *_self, const cc_uint8_t* plaintext, cc_uint32_t plainlen, cc_uint32_t* nr_unpadblock, cc_uint8_t* lastblock);
	int (*unpad)(void *_self, const cc_uint8_t* plaintext, cc_uint32_t plainlen, cc_uint32_t* nr_unpadblock, cc_uint8_t* nr_unpadbyte);
};

typedef struct{
    const void* object;
    cc_uint8_t   blocksize;
    enum blockcipher_dir_e  dir;
    enum blockcipher_mode_e mode;
    struct blockcipher_mode_operations* mode_ops;
	enum blockcipher_pad_e pad;
	struct blockcipher_pad_operations* pad_ops; 
	cc_uint8_t* iv;
	void* data;
}BLOCKCIPHER;


int BlockCipher_SetKey(void* _self, const char* userkey);
int BlockCipher_SetMode(void* _self, enum blockcipher_mode_e mode);
int BlockCipher_SetIV(void* _self, cc_uint8_t* iv, cc_uint8_t iv_len);
int BlockCipher_SetPad(void* _self, enum blockcipher_pad_e pad);
int BlockCipher_ProcessBlock(void* _self, const cc_uint8_t* inblock, cc_uint8_t* outblock);
int BlockCipher_Encryption(void* _self, const cc_uint8_t* plaintext, cc_uint32_t plainlen, cc_uint8_t* cipertext, cc_uint32_t* cipherlen);
int BlockCipher_Decryption(void* _self, const cc_uint8_t* ciphertext, cc_uint32_t cipherlen, cc_uint8_t* plaintext, cc_uint32_t* plainlen);

#endif 