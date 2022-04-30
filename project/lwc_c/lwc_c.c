#include "crypto_aead.h"
#include "stdlib.h"
#include "stdio.h"
#include "api.h"

#ifndef MAIN_ROUNDS
#define MAIN_ROUNDS 1000
#endif

// default kat 137

#ifndef MAIN_MBYTES
#define MAIN_MBYTES 4
#endif

#ifndef MAIN_ADBYTES
#define MAIN_ADBYTES 4
#endif

int main(){
	
    unsigned long long mlen = MAIN_MBYTES;
    unsigned long long adlen = MAIN_ADBYTES;

    unsigned long long *clen = malloc(sizeof(unsigned long long));
    unsigned char *m_in = malloc(MAIN_MBYTES+CRYPTO_ABYTES);
    unsigned char *c = malloc(mlen+CRYPTO_ABYTES);
    unsigned char *ad_in = malloc(MAIN_ADBYTES+CRYPTO_ABYTES);
    unsigned char *nsec_in = malloc(CRYPTO_NSECBYTES);//{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B};
    unsigned char *npub_in = malloc(CRYPTO_NPUBBYTES);//{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B};
    unsigned char *k_in = malloc(CRYPTO_KEYBYTES); //{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};

	for (unsigned char i = 0; i < MAIN_MBYTES; i++) m_in[i] = i;
	for (unsigned char i = 0; i < MAIN_ADBYTES; i++) ad_in[i] = i;
	for (unsigned char i = 0; i < CRYPTO_NSECBYTES; i++) nsec_in[i] = i;
	for (unsigned char i = 0; i < CRYPTO_NPUBBYTES; i++) npub_in[i] = i;
	for (unsigned char i = 0; i < CRYPTO_KEYBYTES; i++) k_in[i] = i;

    const unsigned char *m = m_in;
    const unsigned char *ad = ad_in;
    const unsigned char *nsec = nsec_in;
    const unsigned char *npub = npub_in;
    const unsigned char *k = k_in;

#ifdef DEBUG
    printf("m: ");
    for (unsigned char i = 0; i < mlen; i++) printf("%02x",m[i]);
    printf("\n");

    printf("ad: ");
    for (unsigned char i = 0; i < adlen; i++) printf("%02x",ad[i]);
    printf("\n");

    printf("nsec: ");
	for (unsigned char i = 0; i < CRYPTO_NSECBYTES; i++) printf("%02x",nsec[i]);
	printf("\n");

    printf("npub: ");
	for (unsigned char i = 0; i < CRYPTO_NPUBBYTES; i++) printf("%02x",npub[i]);
	printf("\n");

    printf("k: ");
	for (unsigned char i = 0; i < CRYPTO_KEYBYTES; i++) printf("%02x",k[i]);
	printf("\n");
#endif

    for (int i = 0; i < MAIN_ROUNDS; i++)
        crypto_aead_encrypt(CRYPTO_AEAD_TYPE, c, clen, m, mlen, ad, adlen, nsec, npub, k);

#ifdef DEBUG
    printf("clen: %lld\n", *clen);

    printf("c: ");
    for (int i = 0; i < *clen; i++) printf("%02x", c[i]);
    printf("\n");
#endif

	unsigned char *m_out = malloc(MAIN_MBYTES+CRYPTO_ABYTES);
	unsigned long long *mlen_out = malloc(sizeof(unsigned long long));
	unsigned char *nsec_out = malloc(CRYPTO_NSECBYTES);
    for (int i = 0; i < MAIN_ROUNDS; i++)
	    crypto_aead_decrypt(CRYPTO_AEAD_TYPE, m_out, mlen_out, nsec_out, c, *(unsigned long long *)clen, ad, adlen, npub, k);

#ifdef DEBUG
    printf("mlen_out: %lld\n", *mlen_out);
    printf("m_out: ");
    for (unsigned char i = 0; i < *mlen_out; i++) printf("%02x",m[i]);
    printf("\n");
#endif

    free(clen);
    free(m_in);
    free(c);
    free(ad_in);
    free(nsec_in);
    free(npub_in);
    free(k_in);
    free(m_out);
    free(mlen_out);

    return 0;
}