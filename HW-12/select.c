#include "csapp.h"

int nfds;
fd_set read_set;
rio_t *rp[FD_SETSIZE];

void selectfd(int fd) {
    if (fd >= nfds) nfds = fd + 1;
    FD_SET(fd, &read_set);
}

void unselectfd(int fd) {
    FD_CLR(fd, &read_set);
    while (!FD_ISSET(nfds - 1, &read_set)) --nfds;
}

void conn(int fd) {
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    static char client_hostname[MAXLINE];
    static char client_port[MAXLINE];

    int connfd = Accept(fd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);

    printf("connected to (%s, %s)\n", client_hostname, client_port);

    selectfd(connfd);
    rp[connfd] = Malloc(sizeof(rio_t));
    Rio_readinitb(rp[connfd], connfd);
}

void command() {
    static char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin)) {
        exit(0);
    }
    printf("%s", buf);
}

void echo(int fd) {
    static char buf[MAXLINE];

    int n = Rio_readlineb(rp[fd], buf, MAXLINE);

    if (n <= 0) {
        printf("connection closed\n");
        Free(rp[fd]);
        unselectfd(fd);
        Close(fd);
    } else {
        printf("server received %d bytes\n", n);
        Rio_writen(fd, buf, n);
    }
}

int main(int argc, char **argv) {
    int listenfd, fd;
    fd_set ready_set;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);

    FD_ZERO(&read_set);
    nfds = 0;
    selectfd(STDIN_FILENO);
    selectfd(listenfd);

    while (1) {
        ready_set = read_set;
        Select(nfds, &ready_set, NULL, NULL, NULL);
        for (fd = 0; fd  < nfds; ++fd) {
            if (FD_ISSET(fd, &ready_set)) break;
        }
        if (fd == nfds) continue;
        if (fd == STDIN_FILENO) {
            command();
        } else if (fd == listenfd) {
            conn(fd);
        } else {
            echo(fd);
        }
    }
}
