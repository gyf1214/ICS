#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "pool.h"
#include "util.h"

static Pool pool;

void initPool() {
    debug("init fd pool");

    memset(pool.handler, 0, MaxFD * sizeof(Listener));
    pool.nfds = 0;
    FD_ZERO(&pool.listenSet);
    FD_ZERO(&pool.readySet);
    pool.timeout.tv_sec = Timeout;
    pool.timeout.tv_usec = 0;
}

void listenFD(int fd, Listener l) {
    require(fd > 0 && fd < MaxFD);
    require(l);
    require(!FD_ISSET(fd, &pool.listenSet));
    debug("start listen %d", fd);

    pool.handler[fd] = l;
    FD_SET(fd, &pool.listenSet);
    if (fd + 1 > pool.nfds) pool.nfds = fd + 1;
}

void closeFD(int fd) {
    require(fd > 0 && fd < MaxFD);
    require(FD_ISSET(fd, &pool.listenSet));
    debug("close %d", fd);

    pool.handler[fd] = NULL;
    FD_CLR(fd, &pool.listenSet);
    while (pool.nfds > 0 && !FD_ISSET(pool.nfds - 1, &pool.listenSet)) --pool.nfds;

    close(fd);
}

void eventLoop() {
    debug("start event loop");

    for (;;) {
        debug("select %d fds", pool.nfds);
        pool.readySet = pool.listenSet;
        int ret = select(pool.nfds, &pool.readySet, NULL, NULL, &pool.timeout);
        if (ret < 0) {
            fatal("select failed");
        } else if (ret) {
            int fd;
            for (fd = 0; fd < pool.nfds; ++fd) {
                if (FD_ISSET(fd, &pool.readySet)) {
                    pool.handler[fd](fd);
                }
            }
        } else {
            debug("select timeout, refresh");
        }
    }
}
