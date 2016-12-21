#
# Makefile for Proxy Lab
#
# You may modify is file any way you like (except for the handin
# rule). Autolab will execute the command "make" on your specific
# Makefile to build your proxy from sources.
#
CC = gcc
CFLAGS = -g -Wall -DDEBUG
LDFLAGS = -lpthread

LIBS = proxy.o pool.o buffer.o cache.o
OBJS = $(LIBS) main.o

all : proxy

$(LIBS) : %.o : src/%.h

proxy : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJS) : %.o : src/%.c src/util.h src/config.h makefile
	$(CC) -c $(CFLAGS) -o $@ $<

# Creates a tarball in ../proxylab-handin.tar that you should then
# hand in to Autolab.

handin:
	cp -r src makefile proxylab-handout/
	tar cvf handin.tar proxylab-handout

clean:
	rm -f *~ *.o proxy core *.tar *.zip *.gzip *.bzip *.gz
