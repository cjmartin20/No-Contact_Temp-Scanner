#include "encryptAuth.h"
#include "mbed.h"
#include "mbedtls/cipher.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "temp_sens.h"
#if DEBUG_LEVEL > 0
#include "mbedtls/debug.h"
#endif
#include <string.h>

const unsigned char EncryptAuth::secretKey[16] = 
{   0xf4, 0x82, 0xc6, 0x70, 
    0x3c, 0xc7, 0x61, 0x0a,
    0xb9, 0xa0, 0xb8, 0xe9, 
    0x87, 0xb8, 0xc1, 0x72,
};

const char EncryptAuth::temperature[] = temp_Fareinheit();
//const char EncryptAuth::metadata[] = 0; //person information 

EncryptAuth::EncryptAuth()
{
    memset(ciphertext, 0, sizeof(ciphertext));
    memset(decrypted, 0, sizeof(decrypted));
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&drbg);
    mbedtls_cipher_init(&cipher);
}

EncryptAuth::~EncryptAuth()
{
    memset(ciphertext, 0, sizeof(ciphertext));
    memset(decrypted, 0, sizeof(decrypted));
    mbedtls_cipher_free(&cipher);
    mbedtls_ctr_drbg_free(&drbg);
    mbedtls_entropy_free(&entropy);
}

int EncryptAuth::run()
{
    mbedtls_printf("\n\n");
    print_hex("plaintext message", 
                reinterpret_cast<const unsigned char *>(temperature),
                sizeof(temperature));

    //Pseudo-random num gen using entropy with secret key
    int R = mbedtls_ctr_drbg_seed(&drbg, mbedtls_entropy_func, &entropy, 
                                    secretKey, sizeof(secretKey));

    if(R!=0)
    {
        mbedtls_printf("mbedtls_ctr_debugseed() returned -0x%04x\n", -R);
        return R;
    }
    
    //AES-CCM
    R = mbedtls_cipher_setup(&cipher, 
                            mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_CCM));
    if(R!=0)
    {
        mbedtls_printf("mbedtls_cipher_setup() returned -0x%04x\n", -R);
        return R;
    }

    R = mbedtls_cipher_setkey(&cipher, secretKey, 8 * sizeof(secretKey), MBEDTLS_ENCRYPT);
    if(R!=0)
    {
         mbedtls_printf("mbedtls_cipher_setkey() returned -0x%04X\n", -R);
        return R;
    }

    //Encrypt-authenticate temperature and other data

    const size_t Nonce_len = 8;
    mbedtls_ctr_drbg_random(&drbg, ciphertext, Nonce_len);
    size_t Ciphertext_len = 0;
    
    const size_t Tag_len = 16;
    R = mbedtls_cipher_auth_encrypt(&cipher, ciphertext, Nonce_len,
                                    reinterpret_cast<const unsigned char *>(temperature),
                                    sizeof(temperature),
                                    ciphertext + Nonce_len, &Ciphertext_len, 
                                    ciphertext + Nonce_len + sizeof(temperature),
                                    Tag_len);
    
    if(R!=0)
    {
        mbedtls_printf("mbedtls_cipher_auth_encrypt() returned -0x%04x\n", -R);
        return R;
    }
    Ciphertext_len += Nonce_len + Tag_len;

    print_hex("ciphertext", ciphertext, Ciphertext_len);

    //decrypt-authenticate
    size_t Decrypted_len = 0;
    R = mbedtls_cipher_setkey(&cipher, secretKey, 8*sizeof(secretKey), 
                                MBEDTLS_DECRYPT);
    if(R!=0)
    {
        mbedtls_printf("mbedtls_cipher_setkey() returned -0x%04x\n", -R);
        return R;
    }

    R = mbedtls_cipher_auth_decrypt(&cipher, ciphertext, Nonce_len, 
                                    ciphertext + Nonce_len,
                                    Ciphertext_len - Nonce_len - Tag_len, decrypted,
                                    &Decrypted_len, ciphertext + Ciphertext_len - Tag_len,
                                    Tag_len);
    
    //Checks the return code here 
    if(R == MBEDTLS_ERR_CIPHER_AUTH_FAILED)
    {
        mbedtls_printf("Error here. Data is not authenticated. \n");
        return R;
    }
    else if (R!=0)
    {
        mbedtls_printf("mbedtls_cipher_authdecrypt() returned -0x%04x\n", -R);
        return R;
    }

    print_hex("decrypted", decrypted, Decrypted_len);

    mbedtls_printf("\nDone\n");

    return 0;
}

void EncryptAuth::print_hex(const char *Title, const unsigned char buf[], size_t len)
{
    mbedtls_printf("%s: ", Title);
    for(size_t i = 0; i < len; i ++)
        mbedtls_printf("%02x", buf[i]);
    
    mbedtls_printf("\n");
}

