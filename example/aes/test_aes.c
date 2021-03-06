#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <cryptoc.h>

int main()
{
    int i = 0;

    void* aes = new(Aes, AES_TYPE_128);

    uint8_t* plaintext = "12345";
    uint8_t aeskey[16] = {0x24, 0x75, 0xa2, 0xb3, 0x34, 0x75, 0x56, 0x88, 0x31, 0xe2, 0x12, 0x00, 0x13, 0xaa, 0x54, 0x87};
    uint8_t ciphertext[16] = {0};
    uint8_t plaintext_res[16] = {0};
    uint32_t cipherlen = 0;

    printf("\n=========plaintext =================\n");
    for (i= 0;i < strlen(plaintext); i++)
    {
        printf("%x ",plaintext[i]);
    }
    printf("\n=========plaintext end==============\n");

    BlockCipher_SetKey(aes, aeskey);
    BlockCipher_SetPad(aes, BLOCKCIPHER_PAD_PKCS7);
    BlockCipher_Encryption(aes, plaintext, strlen(plaintext), ciphertext, &cipherlen);

    printf("\n=========cipher ====================\n");
    for (i= 0;i < 16; i++)
    {
        printf("%x ",ciphertext[i]);
    }
    printf("\n=========cipher end=================\n");

    uint32_t plainres_len;
    BlockCipher_Decryption(aes, ciphertext, cipherlen, plaintext_res, &plainres_len);
    printf("plainres_len  %d", plainres_len);
    printf("\n=========plaintext =================\n");
    for (i = 0;i < 16; i++)
    {
        printf("%x ",plaintext_res[i]);
    }
    printf("\n=========plaintext end==============\n");

    delete(aes);

    return 0;	
}

