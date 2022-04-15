CC=gcc
CFLAGS=-I.
DEPS = rc6.h
ODIR=obj
BDIR=bin
_OBJ = rc6.o rc6_test.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rc6_test: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lpthread

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
