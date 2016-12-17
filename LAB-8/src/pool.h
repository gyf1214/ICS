#ifndef __POOL
#define __POOL

#include "config.h"

typedef void (*Listener)(int);

typedef struct {
    Listener handler[MaxFD];
    fd_set listenSet, readySet;
    struct timeval timeout;
    int nfds;
}Pool;

void initPool(void);
void listenFD(int, Listener);
void closeFD(int);
void eventLoop(void);

#endif
