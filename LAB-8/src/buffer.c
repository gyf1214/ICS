#include <unistd.h>
#include <string.h>
#include "buffer.h"
#include "util.h"

void initBuffer(LineBuffer *p, int fd) {
    debug("init line buffer for %d", fd);

    p -> fd = fd;
    p -> next = p -> data;
    p -> remain = 0;
}

int fillBuffer(LineBuffer *p) {
    debug("read from %d", p -> fd);
    require(p -> remain <= MaxLine);

    int ret = read(p -> fd, p -> data + p -> remain, BufSize);
    p -> remain += ret;
    debug("%d get %d bytes, remain %d bytes", p -> fd, ret, p -> remain);
    return ret;
}

const char *readLine(LineBuffer *p) {
    debug("%d flush line", p -> fd)
    int count = 0;
    const char *ret = p -> next;
    for (; *p -> next != '\n' && count < p -> remain; ++p -> next, ++count);
    if (*p -> next == '\n') {
        *p -> next = 0;
        ++count;
        ++p -> next;
        if (count > MaxLine) error("MaxLine exceeded");
        p -> remain -= count;
        debug("%d flush %d bytes, remain %d bytes", count, p -> remain);
        return ret;
    } else {
        if (count > MaxLine) error("MaxLine exceeded");

        memmove(p -> data, ret, count);
        p -> next = p -> data;
        debug("%d no new line, remain %d bytes", count);
        return NULL;
    }
}
