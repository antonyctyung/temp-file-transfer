CC=gcc
CFLAGS=-pg -g -I. -Wall
DEPS = xoodyak_crypto_aead.h photonbeetle_crypto_aead.h api.h crypto_aead.h
ODIR=obj
BDIR=bin
XODIR=extobj
_OBJ = crypto_aead.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
XOBJ = xoodyak/obj/Xoodoo-reference.o xoodyak/obj/Xoodyak.o xoodyak/obj/xoodyak_encrypt.o photonbeetle_128/photonbeetle_128_encrypt.o photonbeetle_128/photon.o

all: xoodyak_c photonbeetle_c xoodyak_c_pth photonbeetle_c_pth

all_max: xoodyak_c_max photonbeetle_c_max xoodyak_c_pth_max photonbeetle_c_pth_max

xoodyak/obj/%.o:
	cd xoodyak && \
	make clean && \
	make xoodyak.a && \
	cd ..

photonbeetle_128/%.o:
	cd photonbeetle_128 && \
	make clean && \
	make photonbeetle_128.a && \
	cd ..

$(ODIR)/%.o: %.c $(DEPS)
	mkdir $(ODIR); $(CC) $(CFLAGS) -c -o $@ $<

# $(ODIR)/%.o: %.c $(DEPS)
# 	mkdir $(ODIR); $(CC) $(CFLAGS) -c -o $@ $< 

xoodyak_c: $(OBJ) $(XOBJ)
	mkdir $(BDIR); $(CC) $(CFLAGS) -DMAKE_XOODYAK -o $(BDIR)/$@ lwc_c.c $^

photonbeetle_c: $(OBJ) $(XOBJ)
	mkdir $(BDIR); $(CC) $(CFLAGS) -DMAKE_PHOTONBEETLE_128 -o $(BDIR)/$@ lwc_c.c $^

xoodyak_c_pth: $(OBJ) $(XOBJ)
	mkdir $(BDIR); $(CC) $(CFLAGS) -DMAKE_XOODYAK -o $(BDIR)/$@ lwc_c_pth.c $^ -lpthread

photonbeetle_c_pth: $(OBJ) $(XOBJ)
	mkdir $(BDIR); $(CC) $(CFLAGS) -DMAKE_PHOTONBEETLE_128 -o $(BDIR)/$@ lwc_c_pth.c $^ -lpthread

xoodyak_c_max: $(OBJ) $(XOBJ)
	mkdir $(BDIR); $(CC) $(CFLAGS) -DMAKE_XOODYAK -DMAIN_ROUNDS=2000000000 -o $(BDIR)/$@ lwc_c.c $^

photonbeetle_c_max: $(OBJ) $(XOBJ)
	mkdir $(BDIR); $(CC) $(CFLAGS) -DMAKE_PHOTONBEETLE_128 -DMAIN_ROUNDS=2000000000 -o $(BDIR)/$@ lwc_c.c $^

xoodyak_c_pth_max: $(OBJ) $(XOBJ)
	mkdir $(BDIR); $(CC) $(CFLAGS) -DMAKE_XOODYAK -DMAIN_ROUNDS=2000000000 -o $(BDIR)/$@ lwc_c_pth.c $^ -lpthread

photonbeetle_c_pth_max: $(OBJ) $(XOBJ)
	mkdir $(BDIR); $(CC) $(CFLAGS) -DMAKE_PHOTONBEETLE_128 -DMAIN_ROUNDS=2000000000 -o $(BDIR)/$@ lwc_c_pth.c $^ -lpthread

.PHONY: clean

clean:
	rm -f gmon.out
	rm -f $(ODIR)/*.o $(BDIR)/*
	rm -f $(XOBJ)