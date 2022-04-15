#ifndef _RC6_H_
#define _RC6_H_

#include <stdint.h>
#include <stddef.h>

typedef unsigned long int WORD; /* Should be 32-bit = 4 bytes        */
#define w        32             /* word size in bits                 */
#define r        20             /* number of rounds                  */  
#define b        16             /* number of bytes in key            */
#define c         4             /* number  words in key = ceil(8*b/w)*/
#define t        44             /* size of table S = 2r+4 words   */
/* Rotation operators. x must be unsigned, to get logical right shift*/
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))

#define lgw       5

#endif