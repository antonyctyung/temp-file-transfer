CC=gcc
CFLAGES=-std=c99 -Wall -Wextra -Wshadow #-fsanitize=address,undefined -static-libasan -O2
OBJS=genkat_aead.o photon.o photonbeetle_128_encrypt.o

all: ref

ref: $(OBJS)
	$(CC) -o ref $(OBJS) $(CFLAGES)

genkat_aead.o: ../../../genkat_aead.c
	$(CC) -c ../../../genkat_aead.c $(CFLAGES) -I./

photonbeetle_128_encrypt.o: photonbeetle_128_encrypt.c
	$(CC) -o photonbeetle_128_encrypt.o -c photonbeetle_128_encrypt.c $(CFLAGES) -I../../../

photon.o: photon.c
	$(CC) -c photon.c $(CFLAGES)

photonbeetle_128.a: photon.o photonbeetle_128_encrypt.o
	ar rcs photonbeetle_128.a photon.o photonbeetle_128_encrypt.o


.PHONY : clean

clean :
#	rm $(OBJS)
	rm -f *.o *.a
