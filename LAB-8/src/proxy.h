#ifndef __PROXY
#define __PROXY

#include "config.h"

typedef struct {
    int dst, src;

}Connection;

extern Connection *conn[MaxFD];

#endif
