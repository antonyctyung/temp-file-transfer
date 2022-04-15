#ifndef _RC4_H_
#define _RC4_H_
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define b        16             /* number of bytes in key            */

void RC4_XENCRYPT(uint8_t* msg_p, size_t length, uint8_t* cypher_p, uint8_t* key);
void RC4_XDECRYPT(uint8_t* cypher_p, size_t length, uint8_t* msg_p, uint8_t* key);

#endif
