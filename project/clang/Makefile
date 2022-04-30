CC=gcc
CFLAGS=-I.
DEPS = rc6.h
ODIR=obj
BDIR=bin
_OBJ = rc6.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: rc6_c rc6_c_pth
all_max: rc6_c_max rc6_c_pth_max

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rc6_c: rc6_c.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

rc6_c_pth: rc6_c_pth.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lpthread

rc6_c_max: rc6_c.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -DROUND=2000000000

rc6_c_pth_max: rc6_c_pth.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lpthread -DROUND=2000000000

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
