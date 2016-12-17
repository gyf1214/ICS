#ifndef __BUFFER
#define __BUFFER

#include "config.h"

typedef struct {
    int fd;
    char data[BufSize + MaxLine];
    char *next;
    int remain;
}LineBuffer;

void initBuffer(LineBuffer *, int);
int fillBuffer(LineBuffer *);
const char *readLine(LineBuffer *);

#endif
