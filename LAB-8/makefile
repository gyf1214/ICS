#
# Makefile for Proxy Lab
#
# You may modify is file any way you like (except for the handin
# rule). Autolab will execute the command "make" on your specific
# Makefile to build your proxy from sources.
#
CC = gcc
CFLAGS = -O2 -Wall -DDEBUG
LDFLAGS = -lpthread

LIBS = proxy.o pool.o buffer.o cache.o
OBJS = $(LIBS) main.o

all : proxy

test: all
	./driver.sh

.PHONY: test all handin clean

$(LIBS) : %.o : src/%.h

proxy : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJS) : %.o : src/%.c src/util.h src/config.h makefile
	$(CC) -c $(CFLAGS) -o $@ $<

# Creates a tarball in ../proxylab-handin.tar that you should then
# hand in to Autolab.

handin: clean
	mkdir proxylab-handout
	cp -r src makefile proxylab-handout/
	tar cvf handin.tar proxylab-handout

clean:
	rm -fr *~ *.o proxy core *.tar *.zip *.gzip *.bzip *.gz *.log
	rm -fr proxylab-handout
