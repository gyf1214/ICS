#ifndef __PROXY
#define __PROXY

#include "config.h"
#include "buffer.h"

enum State {
    url, header, content
};

typedef struct {
    int dst, src;
    LineBuffer buf;
    enum State state;
}Connection;

extern Connection *conn[MaxFD];

void createServer(const char *);

#endif
