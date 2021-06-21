#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>

#ifndef CRYPTO_H
#define CRYPTO_H

#define RSA_KEYLEN 2048
#define AES_KEYLEN 256
#define AES_ROUNDS 6

//#define PSUEDO_CLIENT

#define SUCCESS 0
#define FAILURE -1

#define KEY_SERVER_PRI 0
#define KEY_SERVER_PUB 1
#define KEY_CLIENT_PUB 2
#define KEY_AES        3
#define KEY_AES_IV     4

class Crypto {

public:
    Crypto();

    Crypto(unsigned char *remotePubKey, size_t remotePubKeyLen);

    ~Crypto();


    int aesEncrypt(const unsigned char *msg, size_t msgLen, unsigned char **encMsg);

    int aesDecrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg);

    int writeKeyToFile(FILE *fd, int key);

    int getAESKey(unsigned char **aesKey);

    int setAESKey(unsigned char *aesKey, size_t aesKeyLen);

    int getAESIv(unsigned char **aesIv);

    int setAESIv(unsigned char *aesIv, size_t aesIvLen);

private:
    
    EVP_CIPHER_CTX *aesEncryptCtx;
    EVP_CIPHER_CTX *aesDecryptCtx;

    unsigned char *aesKey;
    unsigned char *aesIV;

    int init();
};

#endif