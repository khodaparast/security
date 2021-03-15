

#ifndef BLOWFISH_H
#define BLOWFISH_H


#include <stddef.h>


#define BLOWFISH_BLOCK_SIZE 8           // Blowfish operates on 8 bytes at a time


typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
   WORD p[18];
   WORD s[4][256];
} BLOWFISH_KEY;


void blowfish_key_setup(const BYTE user_key[], BLOWFISH_KEY *keystruct, size_t len);
void blowfish_encrypt(const BYTE in[], BYTE out[], const BLOWFISH_KEY *keystruct);
void blowfish_decrypt(const BYTE in[], BYTE out[], const BLOWFISH_KEY *keystruct);

#endif   
