#include "Crypto.h"
#include  <chrono>
#include <iterator>

using namespace std;
 



struct bench_data {
  bool done; //is the benchmark is done
  uint64_t object_size; //the size of the objects
  uint64_t op_size;     // the size of the read/write ops
  bool hints;
  // same as object_size for write tests
  int in_flight; //number of reads/writes being waited on
  int started;
  int finished;
  double min_latency;
  double max_latency;
  double avg_latency;
  double latency_diff_sum;
  char *object_contents; //pointer to the contents written to each object
};


static void sanitize_object_contents (bench_data *data, size_t length) {
  // FIPS zeroization audit 20191115: this memset is not security related.
  memset(data->object_contents, 'z', length);
 
}


// unsigned char* benchEncrypt(int& cipher_len){

//     struct bench_data *data=new bench_data;
//     data->op_size =  2048;
//     char* contentsChars = new char[data->op_size];
//     data->object_contents=contentsChars;
//     // unsigned char *ciphertext;

//     sanitize_object_contents(data, data->op_size);
//     std::cout<< "data->object_contents: " << data->object_contents << std::endl;
//     unsigned char *plaintext = (unsigned char *)data->object_contents;
    
//     std::cout<< "----------------------------------------------------------------- " <<  std::endl;
//     std::cout<< "plaintext : " << plaintext<< std::endl;

//     unsigned char* result;

//     // encrypt plaintext
//     Crypto cryptObj;

//     unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
//     unsigned char *iv = (unsigned char *)"0123456789012345";
//     cryptObj.setAESKey(key, 256);
//     cryptObj.setAESIv(iv, 128);

    

//     // encrypt plaintext
//     cipher_len = cryptObj.aesEncrypt(plaintext, data->op_size, &result);
//     std::cout<< "cipher_len: " << cipher_len << std::endl;
//     std::cout<< "plaintext: " << strlen(( char*) plaintext)<< std::endl;
//     std::cout<< "Size of key: " << strlen(( char*) key)<< std::endl;
//     std::cout<< "Size of result inside function: " << cipher_len << std::endl;
//     std::cout<< "result: " << result << std::endl;
//     // result[cipher_len] = '\0';
//     return result;
// }

unsigned char* do_enc(Crypto& cryptObj){

    struct bench_data *data2=new bench_data;
    data2->op_size =  4096;
    char* contentsChars2 = new char[data2->op_size];
    data2->object_contents=contentsChars2;

    sanitize_object_contents(data2, data2->op_size);

     // encrypt plaintext
    unsigned char *plaintext5 = (unsigned char *)data2->object_contents;
    unsigned char *encMsgOut5;
    int encLen5 = cryptObj.aesEncrypt(plaintext5, data2->op_size, &encMsgOut5);

    // std::cout << "encLen5 :  " << encLen5 << std::endl;
    // std::cout << "----------------------------------------" << std::endl;
    // std::cout << "plaintext5 :  " << plaintext5 << std::endl;
    // std::cout << "----------------------------------------" << std::endl;
    // std::cout << "encMsgOut5 :  " << encMsgOut5 << std::endl;
  

    // derypt
    char *decMsg5;
    cryptObj.aesDecrypt(encMsgOut5, encLen5, &decMsg5);
    // std::cout << "----------------------------------------" << std::endl;
    // std::cout << "decMsg5 :  " << decMsg5 << std::endl;
    return encMsgOut5;
}


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

    // unsigned char *aesPass = (unsigned char*)malloc(AES_KEYLEN/8);
    // unsigned char *aesSalt = (unsigned char*)malloc(8);
 
    // if(aesKey == NULL || aesIV == NULL || aesPass == NULL || aesSalt == NULL) {
    //     return FAILURE;
    // }

    // // Get some random data to use as the AES pass and salt
    // if(RAND_bytes(aesPass, AES_KEYLEN/8) == 0) {
    //     return FAILURE;
    // }

    // if(RAND_bytes(aesSalt, 8) == 0) {
    //     return FAILURE;
    // }
 
    // if(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), aesSalt, aesPass, AES_KEYLEN/8, AES_ROUNDS, aesKey, aesIV) == 0) {
    //     return FAILURE;
    // }
 
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


void test2(unsigned char*& b ){

    std::cout << " test 2: before " << b << std::endl;

    b = (unsigned char *)"abcd";

    std::cout << " test 2: after " << b << std::endl;
} 

void test1(unsigned char*& a){

test2(a);
std::cout<< " test 1: " << a << std::endl;

}



int main(int argc, char **argv){

    // unsigned char *c = (unsigned char *)"01234567890123456789012345678901";
    // test1(c);
    // std::cout<< " main: " << c << std::endl;
   


    //-----------------------------------------------------------------------------------------------------

   



    //-----------------------------------------------------------------------------------------------------
    // int plaintext_len7 = 100000;
    // int cipher_len;
    // unsigned char* result= benchEncrypt(cipher_len);
   
    // std::cout<< "result : " << result << std::endl;
    // std::cout << "Size of result outside function: " << cipher_len << std::endl;

   

    // std::cout<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> (0) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<std::endl;


     Crypto cryptObj;
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
    unsigned char *iv = (unsigned char *)"0123456789012345";
    cryptObj.setAESKey(key, 256);
    cryptObj.setAESIv(iv, 128); 

    // unsigned char *encMsgOut5;
     do_enc(cryptObj);
    // std::cout << "encMsgOut5 :  " << encMsgOut5 << std::endl;
    // char *decMsg7;
    // cryptObj.aesDecrypt(result, cipher_len, &decMsg7);
    // std::cout<< "decMsg7 : " << decMsg7 << std::endl;
    //-----------------------------------------------------------------------------------------------------
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

    std::ifstream in2("enc.txt");
    std::string encFile((std::istreambuf_iterator<char>(in2)),
                        std::istreambuf_iterator<char>());

    unsigned char *ciphertext = (unsigned char *)encFile.c_str(); // string --> unsigned char*
    char *decMsg2;
    cryptObj.aesDecrypt(ciphertext, encFile.size(), &decMsg2);

    std::ofstream myDecFile2("dec2.txt", std::ios::out | std::ios::binary);
    myDecFile2 << decMsg2;
    myDecFile2.close();
    // std::cout<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> (1) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<std::endl;



    //-----------------------------------------------------------------------------------------------------
    // decryption (two next lines properly decrypt message, it receives encMsg. 
    // try decryption from file and check functionality)
    // encLen is equal to encFile.size(), in decryption with two different phases use file size
    char *decMsg;
    // cryptObj.aesDecrypt(encMsgOut, encLen, &decMsg);
    cryptObj.aesDecrypt(encMsgOut, encFile.size(), &decMsg);

    std::ofstream myDecFile("dec.txt", std::ios::out | std::ios::binary);
    myDecFile << decMsg;
    myDecFile.close();
    // std::cout<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> (2) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<std::endl;



    //-----------------------------------------------------------------------------------------------------
    struct bench_data *data=new bench_data;
    data->op_size = std::stoi(argv[2]);
    char* contentsChars = new char[data->op_size];
    data->object_contents=contentsChars;

    // sanitize_object_contents(data, data->op_size);
    sanitize_object_contents(data,  data->op_size);

     // encrypt plaintext
    unsigned char *plaintext4 = (unsigned char *)data->object_contents;
    unsigned char *encMsgOut4;
    auto start = chrono::steady_clock::now();
    int encLen4 = cryptObj.aesEncrypt(plaintext4, data->op_size, &encMsgOut4);
    // std::cout << "encLen4 :  " << encLen4 << std::endl;
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    // std::cout << "enccryption time, ns:  " << chrono::duration <double, nano> (diff).count() << " ns" << endl;
    // std::cout << "encryption time, ms:  " << chrono::duration <double, nano> (diff).count()/1000000 << " ms" << endl;
    // std::cout <<"encryption time, s:  " <<chrono::duration <double, nano> (diff).count()/1000000000 << " s" << endl;
        std::cout << encLen4<< ", " <<chrono::duration <double, nano> (diff).count() << ", "<< chrono::duration <double, nano> (diff).count()/1000000 << ", " << chrono::duration <double, nano> (diff).count()/1000000000  << endl;

    // std::cout<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;


 // derypt
    auto start3 = chrono::steady_clock::now();
    char *decMsg4;
    cryptObj.aesDecrypt(encMsgOut4, encLen4, &decMsg4);
    auto end3 = chrono::steady_clock::now();
    auto diff3 = end3 - start3;
    // std::cout << "decryption time, ns:  " <<chrono::duration <double, nano> (diff3).count() << " ns" << endl;
    // std::cout <<  "decryption time, ms:  " <<chrono::duration <double, nano> (diff3).count()/1000000 << " ms" << endl;
    // std::cout << "decryption time, s:  "  <<  chrono::duration <double, nano> (diff3).count()/1000000000 << " s" << endl;

    // std::cout << encLen4<< ", " <<chrono::duration <double, nano> (diff3).count() << ", "<< chrono::duration <double, nano> (diff3).count()/1000000 << ", " << chrono::duration <double, nano> (diff3).count()/1000000000  << endl;
        // std::cout<< " ////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;


    // std::ifstream in3("crypto/build/BigFile.txt");
    // std::string strFile3((std::istreambuf_iterator<char>(in3)),
    //                     std::istreambuf_iterator<char>());
    // auto contents3 = strFile3.c_str();
    // auto message3 = reinterpret_cast<unsigned char *>(const_cast<char *>(contents3));


    // encrypt plaintext
    // auto start2 = chrono::steady_clock::now();
    // size_t messageSize3 = strFile3.size();
    // unsigned char *encMsgOut3;
    // int encLen3 = cryptObj.aesEncrypt(message3, messageSize3, &encMsgOut);
    // auto end2 = chrono::steady_clock::now();
    // auto diff2 = end2 - start2;
    // std::cout << chrono::duration <double, nano> (diff2).count() << " ns" << endl;
    // std::cout << chrono::duration <double, nano> (diff2).count()/1000000 << " ms" << endl;
    // std::cout << chrono::duration <double, nano> (diff2).count()/1000000000 << " s" << endl;
    //     std::cout<<"---------------------------------------" << std::endl;



   







    // std::cout<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> (3) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<std::endl;



    //-----------------------------------------------------------------------------------------------------
    int size = strlen((char*)encMsgOut);
    // std::cout<< "size from encryption: "<< encLen <<" "<< "Size form sizeof() function: "<< size<<std::endl;


    // std::cout<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> (4) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<std::endl;



    // struct bench_data *data2=new bench_data;
    // data2->op_size =  4096;
    // char* contentsChars2 = new char[data2->op_size];
    // data2->object_contents=contentsChars2;

    // sanitize_object_contents(data2, data2->op_size);

    //  // encrypt plaintext
    // unsigned char *plaintext5 = (unsigned char *)data2->object_contents;
    // unsigned char *encMsgOut5;
    // int encLen5 = cryptObj.aesEncrypt(plaintext5, data2->op_size, &encMsgOut5);
    // std::cout << "encLen4 :  " << encLen4 << std::endl;
    // std::cout << "plaintext5 :  " << plaintext5 << std::endl;
  

    // // derypt
    // char *decMsg5;
    // cryptObj.aesDecrypt(encMsgOut5, encLen5, &decMsg5);
    // std::cout << "decMsg5 :  " << decMsg5 << std::endl;

    // std::cout<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> (5) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<std::endl;

   
}