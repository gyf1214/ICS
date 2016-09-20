#
# Makefile that builds btest and other helper programs for the CS:APP data lab
#
CC = gcc
CFLAGS = -O1 -Wall -m32
LIBS = -lm

test: all
	./btest

all: btest

btest: btest.c bits.c decl.c tests.c btest.h bits.h
	$(CC) $(CFLAGS) $(LIBS) -o btest bits.c btest.c decl.c tests.c

clean:
	rm -f *.o btest fshow ishow *~
