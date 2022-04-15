#include "rc4.h"

typedef struct rc4_key
{      
     unsigned char state[256];       
     unsigned char x;        
     unsigned char y;
} rc4_key;
void prepare_key(unsigned char *key_data_ptr,int key_data_len, rc4_key *key);
void rc4(unsigned char *buffer_ptr,int buffer_len,rc4_key * key);
static void swap_byte(unsigned char *a, unsigned char *B);
void prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key)
{
     unsigned char swapByte;
     unsigned char index1;
     unsigned char index2;
     unsigned char* state;
     short counter;     
     
     state = &key->state[0];         
     for(counter = 0; counter < 256; counter++)              
     state[counter] = counter;               
     key->x = 0;     
     key->y = 0;     
     index1 = 0;     
     index2 = 0;             
     for(counter = 0; counter < 256; counter++)      
     {               
          index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;                
          swap_byte(&state[counter], &state[index2]);            
          index1 = (index1 + 1) % key_data_len;  
     }       
 }
 
 void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key)
 { 
     unsigned char x;
     unsigned char y;
     unsigned char* state;
     unsigned char xorIndex;
     short counter;              
     
     x = key->x;     
     y = key->y;     
     
     state = &key->state[0];         
     for(counter = 0; counter < buffer_len; counter ++)      
     {               
          x = (x + 1) % 256;                      
          y = (state[x] + y) % 256;               
          swap_byte(&state[x], &state[y]);                        
               
          xorIndex = state[x] + (state[y]) % 256;                 
               
          buffer_ptr[counter] ^= state[xorIndex];         
      }               
      key->x = x;     
      key->y = y;
 }
 
 static void swap_byte(unsigned char *a, unsigned char *B)
 {
     unsigned char swapByte; 
     
     swapByte = *a; 
     *a = *B;      
     *B = swapByte;
 }

void RC4_XENCRYPT(uint8_t* msg_p, size_t length, uint8_t* cypher_p, uint8_t* key)
{
    rc4_key key4;
    prepare_key(key, b, &key4);
    memcpy(msg_p, cypher_p, length);
    rc4(cypher_p, length, &key4);
}
void RC4_XDECRYPT(uint8_t* cypher_p, size_t length, uint8_t* msg_p, uint8_t* key)
{
    RC4_XENCRYPT(cypher_p, length, msg_p, key);
}