#ifndef _RCX_H_
#define _RCX_H_

#include <stdint.h>
#include <stddef.h>
  
#define b        16             /* number of bytes in key            */
#define c         4             /* number  words in key = ceil(8*b/w)*/

#ifdef MAKE_RC4
#include "rc4.h"
#define encrypt RC4_XENCRYPT
#define decrypt RC4_XDECRYPT
#endif

#ifdef MAKE_RC5
#include "rc5.h"
#define encrypt RC5_XENCRYPT
#define decrypt RC5_XDECRYPT
#endif

#ifdef MAKE_RC6
#include "rc6.h"
#define encrypt RC6_XENCRYPT
#define decrypt RC6_XDECRYPT
#endif

void encrypt(uint8_t* msg_p, size_t length, uint8_t* cypher_p, uint8_t* key);
void decrypt(uint8_t* cypher_p, size_t length, uint8_t* msg_p, uint8_t* key);

#endif