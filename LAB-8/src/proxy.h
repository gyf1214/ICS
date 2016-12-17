#ifndef __PROXY
#define __PROXY

#include "config.h"
#include "buffer.h"

enum State {
    uri, header, content, closed
};

typedef struct {
    int dst, src;
    LineBuffer buf;
    enum State state;
    int content;
}Connection;

typedef struct {
    const char *method, *host, *port, *request;
}URI;

void createServer(const char *);

#endif
