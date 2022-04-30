CC=gcc
CFLAGS=-pg -g -I.
DEPS = Xoodoo.h Xoodoo-SnP.h Cyclist.h Xoodyak.h Cyclist.inc align.h
ODIR=obj
BDIR=bin
_OBJ = Xoodoo-reference.o Xoodyak.o xoodyak_encrypt.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
_AOBJ = Xoodoo-reference.o Xoodyak.o xoodyak_encrypt.o
AOBJ = $(patsubst %,$(ODIR)/%,$(_AOBJ))


# $(ODIR)/Xoodoo-reference.o: Xoodoo-reference.c 
# 	$(CC) $(CFLAGS) -c -o $@ $< 

# $(ODIR)/Xoodyak.o: Xoodoo-reference.o 
# 	$(CC) $(CFLAGS) -c -o $@ $< 

xoodyak.a: $(AOBJ)
	ar rcs $@ $(AOBJ)

$(ODIR)/%.o: %.c $(DEPS)
	mkdir $(ODIR) ; $(CC) $(CFLAGS) -c -o $@ $< 

main: $(OBJ)
	mkdir $(BDIR) ; $(CC) $(CFLAGS) -o $(BDIR)/$@ $^

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
	rm -f *.o *.a
