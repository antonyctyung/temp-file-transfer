#ifndef _RC5_H_
#define _RC5_H_

#include <stdint.h>
#include <stddef.h>

typedef unsigned long int WORD; /* Should be 32-bit = 4 bytes        */
#define w        32             /* word size in bits                 */
#define r        12             /* number of rounds                  */  
#define b        16             /* number of bytes in key            */
#define c         4             /* number  words in key = ceil(8*b/w)*/
#define t        26             /* size of table S = 2*(r+1) words   */
/* Rotation operators. x must be unsigned, to get logical right shift*/
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))

void RC5_XENCRYPT(uint8_t* msg_p, size_t length, uint8_t* cypher_p, uint8_t* key);
void RC5_XDECRYPT(uint8_t* cypher_p, size_t length, uint8_t* msg_p, uint8_t* key);


#endif