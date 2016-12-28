CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread

all : tiny-11.6 tiny-11.8 hello select cgi

select : select.o csapp.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

hello : hello.o csapp.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

tiny-% : tiny-%.o csapp.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o : %.c csapp.h makefile

cgi :
	make -C cgi-bin

clean :
	rm -fr *.o tiny-11.6 tiny-11.8 "select" "hello"

handin:
	(cd ..; tar --exclude cgi-bin --exclude csapp.* -czvf HW-12.tar.gz HW-12)

.PHONY : cgi all clean handin
