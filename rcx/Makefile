CC=gcc
CFLAGS=-I.
DEPS = rc6.h
ODIR=obj
BDIR=bin
_OBJ = rc6.o rc6_test.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: rc4_test rc5_test rc6_test rc4_debug rc5_debug rc6_debug

$(ODIR)/rc4.o: rc4.c rc4.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/rc5.o: rc5.c rc5.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/rc6.o: rc6.c rc6.h
	$(CC) -c -o $@ $< $(CFLAGS)

debug/rc4.o: rc4.c rc4.h
	$(CC) -c -o $@ $< -DDEBUG $(CFLAGS)

debug/rc5.o: rc5.c rc5.h
	$(CC) -c -o $@ $< -DDEBUG $(CFLAGS)

debug/rc6.o: rc6.c rc6.h
	$(CC) -c -o $@ $< -DDEBUG $(CFLAGS)
	
# $(ODIR)/%.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# rc6_test: $(OBJ)
# 	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

rc4_test: obj/rc4.o rc4.h rcx.h
	$(CC) -o $(BDIR)/$@ rcx_test.c $^ -DMAKE_RC4 $(CFLAGS)
	chmod +x $(BDIR)/$@

rc5_test: obj/rc5.o rc5.h rcx.h
	$(CC) -o $(BDIR)/$@ rcx_test.c $^ -DMAKE_RC5 $(CFLAGS)
	chmod +x $(BDIR)/$@

rc6_test: obj/rc6.o rc6.h rcx.h
	$(CC) -o $(BDIR)/$@ rcx_test.c $^ -DMAKE_RC6 $(CFLAGS)
	chmod +x $(BDIR)/$@

rc4_debug: debug/rc4.o rc4.h rcx.h
	$(CC) -o debug/$@ rcx_test.c $^ -DDEBUG -DMAKE_RC4 $(CFLAGS)
	chmod +x debug/$@

rc5_debug: debug/rc5.o rc5.h rcx.h
	$(CC) -o debug/$@ rcx_test.c $^ -DDEBUG -DMAKE_RC5 $(CFLAGS)
	chmod +x debug/$@

rc6_debug: debug/rc6.o rc6.h rcx.h
	$(CC) -o debug/$@ rcx_test.c $^ -DDEBUG -DMAKE_RC6 $(CFLAGS)
	chmod +x debug/$@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/* debug/* 
