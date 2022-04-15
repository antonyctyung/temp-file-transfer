/* modified from RC5 reference code in doi:10.1007/3-540-60590-8_7*/
/* RC5REF.C -- Reference implementation of RC5-32/12/16 in C.        */
/* Copyright (C) 1995 RSA Data Security, Inc.                        */
#include "rc6.h"
#ifdef DEBUG
#include <stdio.h>
#endif

void RC6_ENCRYPT(WORD *pt, WORD *ct, WORD *S) /* 4 WORD input pt/output ct    */
{ 
    #ifdef DEBUG
    printf("Entered RC6_ENCRYPT\n");
    #endif
    WORD i, A=pt[0], B=pt[1]+S[0], C=pt[2], D=pt[3]+S[1], T, U;
  for (i=1; i<=r; i++) 
    {
        T = ROTL(B*(2*B+1),lgw);
        U = ROTL(D*(2*D+1),lgw);
        A = ROTL(A^T,U)+S[2*i]; 
        C = ROTL(C^U,T)+S[2*i+1];
        T = A; A = B; B = C; C = D; D = T;
    }
  ct[0] = A + S[2*r+2]; ct[1] = B; ct[2] = C + S[2*r+3]; ct[3] = D;  
} 

void RC6_DECRYPT(WORD *ct, WORD *pt, WORD *S) /* 4 WORD input ct/output pt    */
{ 
    #ifdef DEBUG
    printf("Entered RC6_DECRYPT\n");
    #endif
    WORD i, A=ct[0]-S[2*r+2], B=ct[1], C=ct[2]-S[2*r+3], D=ct[3], T, U;
    for (i=r; i>0; i--) 
    { 
        T = D; D = C; C = B; B = A; A = T;
        U = ROTL(D*(2*D+1),lgw);         
        T = ROTL(B*(2*B+1),lgw);
        C = ROTR(C-S[2*i+1],T)^U; 
        A = ROTR(A-S[2*i],U)^T; 
    }
    pt[3] = D-S[1]; pt[2] = C; pt[1] = B-S[0]; pt[0] = A;  
} 

void RC6_SETUP(unsigned char *K, WORD *S) /* secret input key K[0...b-1]      */
{  
    #ifdef DEBUG
    printf("Entered RC6_SETUP\n");
    #endif
    WORD P = 0xb7e15163, Q = 0x9e3779b9;  /* magic constants             */
    WORD i, j, k, u=w/8, A, B, L[c]; 
   /* Initialize L, then S, then mix key into S */
   for (i=b-1,L[c-1]=0; i!=-1; i--) L[i/u] = (L[i/u]<<8)+K[i];
   for (S[0]=P,i=1; i<t; i++) S[i] = S[i-1]+Q;
   for (A=B=i=j=k=0; k<3*t; k++,i=(i+1)%t,j=(j+1)%c)   /* 3*t > 3*c */
     { 
        A = S[i] = ROTL(S[i]+(A+B),3);  
        B = L[j] = ROTL(L[j]+(A+B),(A+B)); 
     } 
} 

#define BLKSIZE 4 // 4 words block size

void RC6_XENCRYPT(uint8_t* msg_p, size_t length, uint8_t* cypher_p, uint8_t* key)
{
    for (int i = 0; i < length/BLKSIZE/4; i++)
    {
        WORD S[t];                      /* expanded key table                */
        RC6_SETUP(key, S);
        RC6_ENCRYPT((WORD*)(msg_p+i*BLKSIZE*4), (WORD*)(cypher_p+i*BLKSIZE*4), S);
    }
}

void RC6_XDECRYPT(uint8_t* cypher_p, size_t length, uint8_t* msg_p, uint8_t* key)
{
    for (int i = 0; i < length/BLKSIZE/4; i++)
    {
        WORD S[t];                      /* expanded key table                */
        RC6_SETUP(key, S);
        RC6_DECRYPT((WORD*)(msg_p+i*BLKSIZE*4), (WORD*)(cypher_p+i*BLKSIZE*4), S);
    }
}
