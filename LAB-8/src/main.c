#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "proxy.h"
#include "pool.h"

FILE *stdlog;

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("usage: %s <port>\n", argv[0]);
        exit(0);
    }

    signal(SIGPIPE, SIG_IGN);

#ifdef logPath
    stdlog = fopen(logPath, "a");
#else
    stdlog = stderr;
#endif

    initPool();
    createServer(argv[1]);
    eventLoop();

    return 0;
}
