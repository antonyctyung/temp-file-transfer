#include <stdint.h>
#include <stdlib.h>
// #include <stdio.h>
#include "rc6.h"

uint32_t rotl(uint32_t x, uint32_t amt)
{
    amt = amt&0b11111;
    return (x<<amt) | (x>>(32-amt)); 
}

uint32_t rotr(uint32_t x, uint32_t amt)
{
    amt = amt&0b11111;
    return (x>>amt) | (x<<(32-amt)); 
}


/* 
assuming for simplicity:
w = 32 (32-bit words)
r = 20 (20 rounds)
message is 128 bits
b = 16/24/32 (key size in bytes)
i.e. not for practical usage
*/
void RC6_32_20_32_encrypt(uint8_t* msg_p, size_t b, uint8_t* key)
{
    uint32_t A=0;
    uint32_t B=0;
    uint32_t C=0;
    uint32_t D=0;
    uint32_t t=0;
    uint32_t u=0;
    uint32_t temp = 0;
    const uint32_t lgw = 5;
    const int r = 20;
    // loading message into working registers
    for (int i = 0; i < 4; i++) A = A | ( ((uint32_t) msg_p[i])    << i*8 );
    for (int i = 0; i < 4; i++) B = B | ( ((uint32_t) msg_p[4+i])  << i*8 );
    for (int i = 0; i < 4; i++) C = C | ( ((uint32_t) msg_p[8+i])  << i*8 );
    for (int i = 0; i < 4; i++) D = D | ( ((uint32_t) msg_p[12+i]) << i*8 );

    uint32_t *S = key_expansion(b,key);
    B = B + S[0];
    D = D + S[1];
    for (int i = 1; i <= r; i++)
    {
        t = (B*(2*B+1));
        t = rotl(t,lgw);
        u = (D*(2*D+1));
        u = rotl(u,lgw);
        A = rotl(A^t,u) + S[2*i];
        C = rotl(C^u,t) + S[2*i+1];
        
        temp=A;
        A=B;
        B=C;
        C=D;
        D=temp;
    }
    A = A + S[2*r+2];
    C = C + S[2*r+3];
    free(S);
    // printf("Encrypting: ");
    // for (int i = 0; i < b/2; i++) printf("%02x", msg_p[i]);
    // printf("\n");
    // printf("--> ");
    // for (int i = 0; i < 4; i++)  printf("%02x",((uint8_t)(A>>8*i)));
    // for (int i = 0; i < 4; i++)  printf("%02x",((uint8_t)(B>>8*i)));
    // for (int i = 0; i < 4; i++)  printf("%02x",((uint8_t)(C>>8*i)));
    // for (int i = 0; i < 4; i++)  printf("%02x",((uint8_t)(D>>8*i)));
    // printf("\r\n");
}

void RC6_32_20_32_decrypt(uint8_t* cypher_p, size_t b, uint8_t* key)
{
    uint32_t A=0;
    uint32_t B=0;
    uint32_t C=0;
    uint32_t D=0;
    uint32_t t=0;
    uint32_t u=0;
    uint32_t temp = 0;
    const uint32_t lgw = 5;
    const int r = 20;
    // loading message into working registers
    for (int i = 0; i < 4; i++) A = A | ( ((uint32_t) cypher_p[i])    << i*8 );
    for (int i = 0; i < 4; i++) B = B | ( ((uint32_t) cypher_p[4+i])  << i*8 );
    for (int i = 0; i < 4; i++) C = C | ( ((uint32_t) cypher_p[8+i])  << i*8 );
    for (int i = 0; i < 4; i++) D = D | ( ((uint32_t) cypher_p[12+i]) << i*8 );

    uint32_t *S = key_expansion(b,key);
    C = C - S[2*r+3];
    A = A - S[2*r+2];
    for (int i = r; i >= 1; i--)
    {
        temp=D;
        D=C;
        C=B;
        B=A;
        A=temp;
        u = (D*(2*D+1));
        u = rotl(u,lgw);
        t = (B*(2*B+1));
        t = rotl(t,lgw);
        C = rotr(C-S[2*i+1],t)^u;
        A = rotr(A-S[2*i],u)^t;
    }
    D = D - S[1];
    B = B - S[0];

    // free(S);
    // printf("Encrypting: ");
    // for (int i = 0; i < b/2; i++) printf("%02x", cypher_p[i]);
    // printf("\n");
    // printf("--> ");
    // for (int i = 0; i < 4; i++)  printf("%02x",((uint8_t)(A>>8*i)));
    // for (int i = 0; i < 4; i++)  printf("%02x",((uint8_t)(B>>8*i)));
    // for (int i = 0; i < 4; i++)  printf("%02x",((uint8_t)(C>>8*i)));
    // for (int i = 0; i < 4; i++)  printf("%02x",((uint8_t)(D>>8*i)));
    // printf("\r\n");
}

uint32_t* key_expansion(size_t b, uint8_t* key)
{
    const int r = 20;
    size_t c = b >> 2; // assuming 16/24/32 bytes key
    uint32_t L[c];
    for (int i = 0; i < c; i++) L[i] = 0;
    uint32_t* S = calloc((2*r+4),sizeof(uint32_t)); // 2r+4 = 44 when r = 20
    uint32_t A,B;
    int i,j;

    for (int i = 0; i < c; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            L[i] = L[i] | ( ((uint32_t) key[i*4+j]) << j*8 );
        }
    }

    S[0] = P;

    for (int i = 1; i <= 2*r+3; i++)
    {
        S[i] = S[i-1] + Q;
    }

    i=0;
    j=0;
    A=0;
    B=0;

    for (int s = 1; s <= 3*(2*r+4); s++)
    {
        S[i] = rotl(S[i]+A+B, 3);
        A = S[i];
        L[j] = rotl(L[j]+A+B, A+B);
        B = L[j];
        i = (i+1)%(2*r+4);
        j = (j+1)%c;
    }

    return S;
}


// int main()
// {
//     uint8_t plaintext[16] = {   0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79, 
//                                 0x8a, 0x9b, 0xac, 0xbd, 0xce, 0xdf, 0xe0, 0xf1  };
//     uint8_t cyphertext[16] = {  0xc8, 0x24, 0x18, 0x16, 0xf0, 0xd7, 0xe4, 0x89,
//                                 0x20, 0xad, 0x16, 0xa1, 0x67, 0x4e, 0x5d, 0x48  };
//     uint8_t key[32] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 
//                         0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
//                         0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, 
//                         0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe  };
//     uint32_t* S = key_expansion(32,key);
//     RC6_32_20_32_encrypt(plaintext,32,key); //c824 ... 5d48
//     RC6_32_20_32_decrypt(cyphertext,32,key); //0213 ... e0f1
//     return 1;
// }
