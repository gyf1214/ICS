#
# Makefile for the CS:APP Shell Lab
#
# Type "make" to build your shell and driver
#
CC = /usr/bin/gcc
CFLAGS = -Wall -g -Werror

FILES = sdriver runtrace tsh myspin1 myspin2 myenv myintp myints mytstpp mytstps mysplit mysplitp mycat

TEST = 17

test: all
	./sdriver -t $(TEST) 2> /dev/null

all: $(FILES)

test-all: all
	./sdriver 2> /dev/null | grep -E 'Oops|Score'

#
# Using link-time interpositioning to introduce non-determinism in the
# order that parent and child execute after invoking fork
#
tsh: tsh.c fork.c
	$(CC) $(CFLAGS)   -Wl,--wrap,fork -o tsh tsh.c fork.c $(LIBS)

sdriver: sdriver.o
sdriver.o: sdriver.c config.h
runtrace.o: runtrace.c config.h

# Clean up
clean:
	rm -f $(FILES) *.o *~
