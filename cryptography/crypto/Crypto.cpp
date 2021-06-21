#include "Crypto.h"
 
using namespace std;
 

Crypto::Crypto() {
      
    init();
}
 

 
Crypto::~Crypto() {
 
    EVP_CIPHER_CTX_cleanup(aesEncryptCtx);
    EVP_CIPHER_CTX_cleanup(aesDecryptCtx);
 
    free(aesEncryptCtx);
    free(aesDecryptCtx);
 
    free(aesKey);
    free(aesIV);
}

  
int Crypto::writeKeyToFile(FILE *fd, int key) {
    switch(key) {
        case KEY_AES:
            fwrite(aesKey, 1, AES_KEYLEN, fd);
            break;

        case KEY_AES_IV:
            fwrite(aesIV, 1, AES_KEYLEN, fd);
            break;

        default:
            return FAILURE;
    }
 
    return SUCCESS;
}
 
 
int Crypto::getAESKey(unsigned char **aesKey) {
    *aesKey = this->aesKey;
    return AES_KEYLEN/8;
}
 
int Crypto::setAESKey(unsigned char *aesKey, size_t aesKeyLen) {
    // Ensure the new key is the proper size
    if((int)aesKeyLen != AES_KEYLEN/8) {
        return FAILURE;
    }
 
    strncpy((char*)this->aesKey, (const char*)aesKey, AES_KEYLEN/8);
 
    return SUCCESS;
}

int Crypto::getAESIv(unsigned char **aesIV) {
    *aesIV = this->aesIV;
    return AES_KEYLEN/16;
}
 
int Crypto::setAESIv(unsigned char *aesIV, size_t aesIVLen) {
    // Ensure the new IV is the proper size
    if((int)aesIVLen != AES_KEYLEN/16) {
        return FAILURE;
    }
 
    strncpy((char*)this->aesIV, (const char*)aesIV, AES_KEYLEN/16);
 
    return SUCCESS;
}
 
int Crypto::init() {
    // Initalize contexts
    // rsaEncryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
    aesEncryptCtx = EVP_CIPHER_CTX_new();
 
    // aesDecryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
    aesDecryptCtx = EVP_CIPHER_CTX_new();
 
    // Always a good idea to check if malloc failed
    if(aesEncryptCtx == NULL || aesDecryptCtx == NULL) {
        return FAILURE;
    }
 
    // Init these here to make valgrind happy
    // EVP_CIPHER_CTX_init(rsaEncryptCtx);
    EVP_CIPHER_CTX_init(aesEncryptCtx);
 
    // EVP_CIPHER_CTX_init(rsaDecryptCtx);
    EVP_CIPHER_CTX_init(aesDecryptCtx);
     
 
    // Init AES
    aesKey = (unsigned char*)malloc(AES_KEYLEN/8);
    aesIV = (unsigned char*)malloc(AES_KEYLEN/8);

    unsigned char *aesPass = (unsigned char*)malloc(AES_KEYLEN/8);
    unsigned char *aesSalt = (unsigned char*)malloc(8);
 
    if(aesKey == NULL || aesIV == NULL || aesPass == NULL || aesSalt == NULL) {
        return FAILURE;
    }

    // Get some random data to use as the AES pass and salt
    if(RAND_bytes(aesPass, AES_KEYLEN/8) == 0) {
        return FAILURE;
    }

    if(RAND_bytes(aesSalt, 8) == 0) {
        return FAILURE;
    }
 
    if(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), aesSalt, aesPass, AES_KEYLEN/8, AES_ROUNDS, aesKey, aesIV) == 0) {
        return FAILURE;
    }
 
    return SUCCESS;
}
 

int Crypto::aesEncrypt(const unsigned char *msg, size_t msgLen, unsigned char **encMsg) {
    size_t blockLen  = 0;
    size_t encMsgLen = 0;
 
    *encMsg = (unsigned char*)malloc(msgLen + AES_BLOCK_SIZE);
    if(encMsg == NULL) return FAILURE;
 
    if(!EVP_EncryptInit_ex(aesEncryptCtx, EVP_aes_256_cbc(), NULL, aesKey, aesIV)) {
        return FAILURE;
    }
 
    if(!EVP_EncryptUpdate(aesEncryptCtx, *encMsg, (int*)&blockLen, (unsigned char*)msg, msgLen)) {
        return FAILURE;
    }
    encMsgLen += blockLen;
 
    if(!EVP_EncryptFinal_ex(aesEncryptCtx, *encMsg + encMsgLen, (int*)&blockLen)) {
        return FAILURE;
    }
 
    EVP_CIPHER_CTX_cleanup(aesEncryptCtx);
 
    return encMsgLen + blockLen;
}

int Crypto::aesDecrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg) {
    size_t decLen   = 0;
    size_t blockLen = 0;
 
    *decMsg = (char*)malloc(encMsgLen);
    if(*decMsg == NULL) return FAILURE;
 
    if(!EVP_DecryptInit_ex(aesDecryptCtx, EVP_aes_256_cbc(), NULL, aesKey, aesIV)) {
        return FAILURE;
    }
 
    if(!EVP_DecryptUpdate(aesDecryptCtx, (unsigned char*)*decMsg, (int*)&blockLen, encMsg, (int)encMsgLen)) {
        return FAILURE;
    }
    decLen += blockLen;
 
    if(!EVP_DecryptFinal_ex(aesDecryptCtx, (unsigned char*)*decMsg + decLen, (int*)&blockLen)) {
        return FAILURE;
    }
    decLen += blockLen;
 
    (*decMsg)[decLen] = '\0';
 
    EVP_CIPHER_CTX_cleanup(aesDecryptCtx);
 
    return encMsgLen;
}




int main(int argc, char **argv){

// file encryption up to 9MB has been tested with this script

    Crypto cryptObj;

 /* A 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";
    cryptObj.setAESKey(key, 256);
    cryptObj.setAESIv(iv, 128);

    std::cout<< "File name >>>>>>>>>>>>>" <<argv[1]<<std::endl;


    // read input file, convert it to unsigned char array 
    std::ifstream in(argv[1]);
    std::string strFile((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
    auto contents = strFile.c_str();
    auto message = reinterpret_cast<unsigned char *>(const_cast<char *>(contents));


    // encrypt plaintext
    size_t messageSize = strFile.size();
    unsigned char *encMsgOut;
    int encLen = cryptObj.aesEncrypt(message, messageSize, &encMsgOut);


    // write encrypted message into a file (enc.txt)
    std::ofstream myEncFile("enc.txt", std::ios::out | std::ios::binary);
    myEncFile.write((char *)&encMsgOut[0], encLen);
    myEncFile.close();


    std::cout<< "plaintext size : "<< messageSize<<std::endl;
    std::ifstream in2("enc.txt");
    std::string encFile((std::istreambuf_iterator<char>(in2)),
                        std::istreambuf_iterator<char>());
    std::cout<< "enc file size : "<< encFile.size()<<std::endl;
    std::cout<< "encLen size : "<< encLen<<std::endl;


    // decryption (two next lines properly decrypt message, it receive encMsg. 
    // try decryption from file and check functionality)
    // encLen is equal to encFile.size(), in decryption with two different phases use file size
    char *decMsg;
    // cryptObj.aesDecrypt(encMsgOut, encLen, &decMsg);
    cryptObj.aesDecrypt(encMsgOut, encFile.size(), &decMsg);

    std::ofstream myDecFile("dec.txt", std::ios::out | std::ios::binary);
    myDecFile << decMsg;
    myDecFile.close();







    unsigned char *ciphertext = (unsigned char *)encFile.c_str(); // string --> unsigned char*
    char *decMsg2;
    cryptObj.aesDecrypt(ciphertext, encFile.size(), &decMsg2);

    std::ofstream myDecFile2("dec2.txt", std::ios::out | std::ios::binary);
    myDecFile2 << decMsg2;
    myDecFile2.close();




    std::cout<<"<<<<<<<<<<< ( 5 ) >>>>>>>>>>>>"<<std::endl;

}