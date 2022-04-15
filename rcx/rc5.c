// source: doi:10.1007/3-540-60590-8_7
/* RC5REF.C -- Reference implementation of RC5-32/12/16 in C.        */
/* Copyright (C) 1995 RSA Data Security, Inc.                        */
#include "rc5.h"
#ifdef DEBUG
#include <stdio.h>
#endif

void RC5_ENCRYPT(WORD *pt, WORD *ct, WORD *S) /* 2 WORD input pt/output ct    */
{ 
  #ifdef DEBUG
  printf("Entered RC5_ENCRYPT\n");
  #endif
    WORD i, A=pt[0]+S[0], B=pt[1]+S[1];
  for (i=1; i<=r; i++) 
    { A = ROTL(A^B,B)+S[2*i]; 
      B = ROTL(B^A,A)+S[2*i+1]; 
    }
  ct[0] = A; ct[1] = B;  
} 

void RC5_DECRYPT(WORD *ct, WORD *pt, WORD *S) /* 2 WORD input ct/output pt    */
{ 
  #ifdef DEBUG
  printf("Entered RC5_DECRYPT\n");
  #endif
  WORD i, B=ct[1], A=ct[0];
  for (i=r; i>0; i--) 
    { B = ROTR(B-S[2*i+1],A)^A; 
      A = ROTR(A-S[2*i],B)^B; 
    }
  pt[1] = B-S[1]; pt[0] = A-S[0];  
} 

void RC5_SETUP(unsigned char *K, WORD *S) /* secret input key K[0...b-1]      */
{  
  #ifdef DEBUG
  printf("Entered RC5_SETUP\n");
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

#define BLKSIZE 2 // 4 words block size

void RC5_XENCRYPT(uint8_t* msg_p, size_t length, uint8_t* cypher_p, uint8_t* key)
{
    for (int i = 0; i < length/BLKSIZE/4; i++)
    {
        WORD S[t];                      /* expanded key table                */
        RC5_SETUP(key, S);
        RC5_ENCRYPT((WORD*)(msg_p+i*BLKSIZE*4), (WORD*)(cypher_p+i*BLKSIZE*4), S);
    }
}

void RC5_XDECRYPT(uint8_t* cypher_p, size_t length, uint8_t* msg_p, uint8_t* key)
{
    for (int i = 0; i < length/BLKSIZE/4; i++)
    {
        WORD S[t];                      /* expanded key table                */
        RC5_SETUP(key, S);
        RC5_DECRYPT((WORD*)(msg_p+i*BLKSIZE*4), (WORD*)(cypher_p+i*BLKSIZE*4), S);
    }
}
