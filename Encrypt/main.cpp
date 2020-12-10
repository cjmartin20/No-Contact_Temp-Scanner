#include "mbed.h"
#include "encryptAuth.h"
#include "mbedtls/platform.h"

int main()
{
    int exitKey = MBEDTLS_EXIT_SUCCESS;
    int R; //used to print the errors

    if((R=mbedtls_platform_setup(NULL))!=0)
    {
        printf("Platfrom init failed w/ error%ds\n", R);
        return MBEDTLS_EXIT_FAILURE;
    }

    EncryptAuth *encryptAuth = new EncryptAuth();

    if((R=encryptAuth->run())!=0)
    {
        mbedtls_printf("Example failed w/ error %d\n", R);
        exitKey = MBEDTLS_EXIT_FAILURE;
    }

    delete encryptAuth;

    mbedtls_platform_teardown(NULL);
    return exitKey;
}

