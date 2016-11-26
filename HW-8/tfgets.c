#include <unistd.h>
#include <setjmp.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#define DEBUG
#define TIMEOUT 5

static jmp_buf env_wakeup;

static void wakeup(int sig) {

#ifdef DEBUG
    printf("tfgets: timeout\n");
#endif

    siglongjmp(env_wakeup, 1);
}

char *tfgets(char *s, int n, FILE *stream) {
    static sig_t old_handler;

    if (sigsetjmp(env_wakeup, 1)) {
        alarm(0);
        signal(SIGALRM, old_handler);
        return NULL;
    } else {
        old_handler = signal(SIGALRM, wakeup);
        alarm(TIMEOUT);
        char *ret = fgets(s, n, stream);
        alarm(0);
        signal(SIGALRM, old_handler);
        return ret;
    }
}

#ifdef DEBUG
#define BUFSIZE 100

char buf[BUFSIZE];

int main() {
    char *ret = tfgets(buf, BUFSIZE, stdin);
    if (ret) printf("%s", buf);
    return 0;
}

#endif
