#include "csapp.h"

void *thread(void *);

int main(int argc, char **argv) {
    if (argc <= 1) exit(-1);

    int n = atoi(argv[1]);
    pthread_t *tids = malloc(n * sizeof(pthread_t));
    int i;

    for (i = 0; i < n; ++i) {
        Pthread_create(tids + i, NULL, thread, NULL);
    }
    for (i = 0; i < n; ++i) {
        Pthread_join(tids[i], NULL);
    }

    exit(0);
}

void *thread(void *vargp) {
    printf("Hello, world!\n");
    return NULL;
}
