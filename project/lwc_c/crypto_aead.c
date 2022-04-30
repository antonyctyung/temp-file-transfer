#include "crypto_aead.h"
#include "api.h"
//#include "tinyjambu_crypto_aead.h"
#include "photonbeetle_crypto_aead.h"
#include "xoodyak_crypto_aead.h"

int crypto_aead_encrypt(
    AEADType type,
    unsigned char *c, unsigned long long *clen,
    const unsigned char *m, unsigned long long mlen,
    const unsigned char *ad, unsigned long long adlen,
    const unsigned char *nsec,
    const unsigned char *npub,
    const unsigned char *k
){
    switch (type)
    {
        case XOODYAK_AEAD:
            xoodyak_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
            break;
        // case TINYJAMBU_128_AEAD:
        //     tinyjambu_128_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
        //     break;
        // case TINYJAMBU_192_AEAD:
        //     tinyjambu_192_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
        //     break;
        // case TINYJAMBU_256_AEAD:
        //     tinyjambu_256_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
        //     break;
        case PHOTONBEETLE_128_AEAD:
            photonbeetle_128_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
            break;
    }
}

int crypto_aead_decrypt(
    AEADType type,
    unsigned char *m, unsigned long long *mlen,
    unsigned char *nsec,
    const unsigned char *c, unsigned long long clen,
    const unsigned char *ad, unsigned long long adlen,
    const unsigned char *npub,
    const unsigned char *k
){
    switch (type)
    {
        case XOODYAK_AEAD:
            xoodyak_crypto_aead_decrypt(m, mlen, nsec, c, clen, ad, adlen, npub, k);
            break;
        // case TINYJAMBU_128_AEAD:
        //     tinyjambu_128_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
        //     break;
        // case TINYJAMBU_192_AEAD:
        //     tinyjambu_192_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
        //     break;
        // case TINYJAMBU_256_AEAD:
        //     tinyjambu_256_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
        //     break;
        case PHOTONBEETLE_128_AEAD:
            photonbeetle_128_crypto_aead_decrypt(m, mlen, nsec, c, clen, ad, adlen, npub, k);
            break;
    }
}
