#include "rcx.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#define ROUND 1000
#define MSGBYTE 32

int main()
{
    // uint8_t plaintext[16] = {   0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79, 
    //                             0x8a, 0x9b, 0xac, 0xbd, 0xce, 0xdf, 0xe0, 0xf1  };
    // uint8_t cyphertext[16] = {  0xc8, 0x24, 0x18, 0x16, 0xf0, 0xd7, 0xe4, 0x89,
    //                             0x20, 0xad, 0x16, 0xa1, 0x67, 0x4e, 0x5d, 0x48  };
    // uint8_t aftertext[16] = {   0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79, 
    //                             0x8a, 0x9b, 0xac, 0xbd, 0xce, 0xdf, 0xe0, 0xf1  };
    // uint8_t key[32] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 
    //                     0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
    //                     0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, 
    //                     0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe  };
    uint8_t *plaintext = malloc(MSGBYTE);    
    uint8_t *ciphertext = malloc(MSGBYTE);    
    uint8_t *aftertext = malloc(MSGBYTE);    
    uint8_t *key = malloc(b);    
    for (int i = 0; i < ROUND; i++)
    {
        int fd = open("/dev/urandom", O_RDONLY);
        read(fd, plaintext, MSGBYTE);
        read(fd, key, b);
        close(fd);
        
        #ifdef DEBUG
            printf("CASE %3d: \n", i);
            printf("PT: ");
            for (int i = 0; i < MSGBYTE; i++) printf("%02x",plaintext[i]);
            printf("\n");
        #endif
        
        encrypt(plaintext,MSGBYTE,ciphertext,key); //c824 ... 5d48
        
        #ifdef DEBUG
            printf("CT: ");
            for (int i = 0; i < MSGBYTE; i++) printf("%02x",ciphertext[i]);
            printf("\n");
        #endif

        decrypt(ciphertext,MSGBYTE,aftertext,key); //0213 ... e0f1

        #ifdef DEBUG
            printf("AT: ");
            for (int i = 0; i < MSGBYTE; i++) printf("%02x",ciphertext[i]);
            printf("\n");
            printf("\n");
        #endif

        // for (int i = 0; i < MSGBYTE; i++)
        // {
        //     assert(plaintext[i] == aftertext[i]);
        // }
    }
    return 1;
}
