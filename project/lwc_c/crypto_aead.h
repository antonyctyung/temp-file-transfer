#ifndef _CRYPTO_AEAD_H_
#define _CRYPTO_AEAD_H_

typedef enum e_AEADType
{
    XOODYAK_AEAD = 0,
    // TINYJAMBU_128_AEAD,
    // TINYJAMBU_192_AEAD,
    // TINYJAMBU_256_AEAD,
    PHOTONBEETLE_128_AEAD
} AEADType;

int crypto_aead_encrypt(
    AEADType type,
    unsigned char *c, unsigned long long *clen,
    const unsigned char *m, unsigned long long mlen,
    const unsigned char *ad, unsigned long long adlen,
    const unsigned char *nsec,
    const unsigned char *npub,
    const unsigned char *k
);

int crypto_aead_decrypt(
    AEADType type,
    unsigned char *m, unsigned long long *mlen,
    unsigned char *nsec,
    const unsigned char *c, unsigned long long clen,
    const unsigned char *ad, unsigned long long adlen,
    const unsigned char *npub,
    const unsigned char *k
);

#endif
