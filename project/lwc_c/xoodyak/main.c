#include "xoodyak_crypto_aead.h"

#ifndef MAIN_MBYTES
#define MAIN_MBYTES 4
#endif

#ifndef MAIN_ADBYTES
#define MAIN_ADBYTES 4
#endif

#define MAIN_ROUNDS 1

#include "stdlib.h"
#include "stdio.h"
#include "api.h"

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

    for (int i = 0; i < MAIN_ROUNDS; i++)
    xoodyak_crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);

    printf("c: ");
    for (int i = 0; i < *clen; i++) printf("%02x", c[i]);
    printf("\n");

    // printf("m: ");
    // for (unsigned char i = 0; i < mlen; i++) printf("%02x",m[i]);
    // printf("\n");

    // printf("ad: ");
    // for (unsigned char i = 0; i < adlen; i++) printf("%02x",ad[i]);
    // printf("\n");

    // printf("nsec: ");
	// for (unsigned char i = 0; i < 12; i++) printf("%02x",nsec[i]);
	// printf("\n");

    // printf("npub: ");
	// for (unsigned char i = 0; i < 12; i++) printf("%02x",npub[i]);
	// printf("\n");

    // printf("k: ");
	// for (unsigned char i = 0; i < 16; i++) printf("%02x",k[i]);
	// printf("\n");
	// unsigned char *m_out = malloc(1024);
	// unsigned long long *mlen_out = malloc(sizeof(unsigned long long));
	// printf("before passing \n");
	// // tinyjambu_128_crypto_aead_decrypt(m_out, mlen_out, c, *clen, ad, adlen, nsec, npub, k);
	// printf("after passing \n");
	// for (unsigned char i = 0; i < 12; i++) printf("%02x",nsec[i]);
	// printf("\n");
	// for (unsigned char i = 0; i < 12; i++) printf("%02x",npub[i]);
	// printf("\n");
	// for (unsigned char i = 0; i < 16; i++) printf("%02x",k[i]);
	// printf("\n");

    // for (int i = 0; i < *mlen_out; i++)
    // {
    //     printf("%x\n", m_out[i]);
    // }

    free(clen);
    free(m_in);
    free(c);
    free(ad_in);
    free(nsec_in);
    free(npub_in);
    free(k_in);

    return 0;
}
