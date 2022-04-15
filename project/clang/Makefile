CC=gcc
CFLAGS=-I.
DEPS = rc6.h
ODIR=obj
BDIR=bin
_OBJ = rc6.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: rc6_test rc6_test_pth

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rc6_test: rc6_test.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

rc6_test_pth: rc6_test_pth.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lpthread


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
