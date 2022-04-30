CC=gcc
CFLAGS=-I.
DEPS = rc6.h
ODIR=obj
BDIR=bin
_OBJ = rc6.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: rc6_c rc6_c_pth
all_loop: rc6_c_loop rc6_c_pth_loop

$(ODIR)/%.o: %.c $(DEPS)
	mkdir $(ODIR) ; $(CC) -c -o $@ $< $(CFLAGS)

rc6_c: rc6_c.c $(OBJ)
	mkdir $(BDIR) ; $(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

rc6_c_pth: rc6_c_pth.c $(OBJ)
	mkdir $(BDIR) ; $(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lpthread

rc6_c_loop: rc6_c.c $(OBJ)
	mkdir $(BDIR) ; $(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -DMAIN_LOOP

rc6_c_pth_loop: rc6_c_pth.c $(OBJ)
	mkdir $(BDIR) ; $(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lpthread -DMAIN_LOOP

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
