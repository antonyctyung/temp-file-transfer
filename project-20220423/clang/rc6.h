#ifndef _RC6_H_
#define _RC6_H_

#include <stdint.h>
#include <stdlib.h>

#define P 0xb7e15163
#define Q 0x9e3779b9

/* 
assuming for simplicity:
w = 32 (32-bit words)
r = 20 (20 rounds)
message is 128 bits
b = 16/24/32 (key size in bytes)
i.e. not for practical usage
*/
void RC6_32_20_32_encrypt(uint8_t* msg_p, size_t b, uint8_t* key);
void RC6_32_20_32_decrypt(uint8_t* cypher_p, size_t b, uint8_t* key);
uint32_t* key_expansion(size_t b, uint8_t* key);

#endif