#include <stdint.h>
#include "rc6.h"
#include "pthread.h" 
#include <stdio.h>
#include <stdlib.h>

#define ROUND 1000
#define THRDCNT 8

void *uut()
{
    uint8_t plaintext[16] = {   0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79, 
                                0x8a, 0x9b, 0xac, 0xbd, 0xce, 0xdf, 0xe0, 0xf1  };
    uint8_t cyphertext[16] = {  0xc8, 0x24, 0x18, 0x16, 0xf0, 0xd7, 0xe4, 0x89,
                                0x20, 0xad, 0x16, 0xa1, 0x67, 0x4e, 0x5d, 0x48  };
    uint8_t key[32] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 
                        0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
                        0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, 
                        0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe  };
    RC6_32_20_32_encrypt(plaintext,32,key); //c824 ... 5d48
    RC6_32_20_32_decrypt(cyphertext,32,key); //0213 ... e0f1
}

int main()
{
    pthread_t *pthrd;
    pthrd = malloc(THRDCNT*sizeof(pthread_t));

    for (int i = 0; i < ROUND/THRDCNT; i++)
    {
        for (int j = 0; j < THRDCNT; j++)
        {
            pthread_create(&(pthrd[i]), NULL, uut, NULL);
        }
        for (int j = 0; j < THRDCNT; j++)
        {
            pthread_join(pthrd[i], NULL);
        }
        // printf("%3d/125\n", i+1);
    }
    // for (int i = 0; i < ROUND; i++) uut();
    return 1;
}
