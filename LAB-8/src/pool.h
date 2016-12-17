#ifndef __POOL
#define __POOL

#include <sys/select.h>
#include "config.h"

typedef void (*Listener)(int, int, const char *);

typedef struct {
    Listener handler[MaxFD];
    fd_set listenSet, readySet;
    struct timeval timeout;
    int nfds;
}Pool;

extern Pool pool;

void initPool(void);
void listenFD(int, Listener);
void closeFD(int);
void eventLoop(void);

#endif
