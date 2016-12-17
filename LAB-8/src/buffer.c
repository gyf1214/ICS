#include <unistd.h>
#include <string.h>
#include "buffer.h"
#include "util.h"

void initBuffer(LineBuffer *p, int fd) {
    p -> fd = fd;
    p -> next = p -> data;
    p -> remain = 0;
}

int fillBuffer(LineBuffer *p) {
    require(p -> remain <= MaxLine);

    int ret = read(p -> fd, p -> data + p -> remain, BufSize);
    p -> remain += ret;
    return ret;
}

char *readLine(LineBuffer *p) {
    int count = 0;
    char *ret = p -> next;
    for (; *p -> next != '\n' && count < p -> remain; ++p -> next, ++count);
    if (*p -> next == '\n') {
        ++p -> next;
        p -> remain -= count + 1;
        return ret;
    } else {
        if (count > MaxLine) error("MaxLine exceeded");
        
        memmove(p -> data, ret, count);
        p -> next = p -> data;
        return NULL;
    }
}
