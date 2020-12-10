#ifndef _AUTHCRYPT_H_
#define _AUTHCRYPT_H_
#include "mbedtls/cipher.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/platform.h"
#include "temp_sens.h"

//authenticates encyrption class using mbed tls
class EncryptAuth
{
    public: 
        EncryptAuth(); //constructs an EncryptAuth instance
        ~EncryptAuth(); //frees up any allocated resources
        int run(); // runs the authenticated encryption example
        
    private:
        //prints in hexadecimal 
        //Title is printed before hex string
        //buf[] is the buffer to print the hex
        //and len is the length of the buffer
        void print_hex(const char *Title, const unsigned char buf[], size_t len);
        //pre-shared key that should be randomly generated
        static const unsigned char secretKey[16];
        //temperature data that should be protected
        static const char temperature[];
        //metadata that is transmitted and authenticated
        //static const char metadata[];
        //ciphertext buffer that has enough space to hold temp data + Nonce + Tag
        unsigned char ciphertext[128];
        //plaintext buffer w/ enough space to hold decrypted temp data
        unsigned char decrypted[128];
        //PRNG entropy pool
        mbedtls_ctr_drbg_context drbg;
        //block cipher config
        mbedtls_cipher_context_t cipher;
        
        mbedtls_entropy_context entropy;
};

#endif /* _AUTHCRYPT_H */