#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "proxy.h"
#include "pool.h"

#ifdef logPath
FILE *logFile;
#endif

/* Recommended max cache and object sizes */
// #define MAX_CACHE_SIZE 1049000
// #define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
// static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("usage: %s <port>\n", argv[0]);
        exit(0);
    }

    signal(SIGPIPE, SIG_IGN);

#ifdef logPath
    logFile = fopen(logPath, "a");
#endif

    initPool();
    createServer(argv[1]);
    eventLoop();

    return 0;
}
