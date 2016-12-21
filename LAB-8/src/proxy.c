#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include "proxy.h"
#include "util.h"
#include "pool.h"

static Connection *conn[MaxFD];

/* $begin open_listenfd */
static int open_listenfd(const char *port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, optval=1;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
    hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */
    getaddrinfo(NULL, port, &hints, &listp);

    /* Walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;  /* Socket failed, try the next */

        /* Eliminates "Address already in use" error from bind */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,    //line:netp:csapp:setsockopt
                   (const void *)&optval , sizeof(int));

        /* Bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; /* Success */
        close(listenfd); /* Bind failed, try the next */
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* No address worked */
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, ListenQ) < 0) {
        close(listenfd);
	return -1;
    }
    return listenfd;
}
/* $end open_listenfd */

/* $begin open_clientfd */
static int open_clientfd(const char *hostname, const char *port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV;  /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG;  /* Recommended for connections */
    getaddrinfo(hostname, port, &hints, &listp);

    /* Walk the list for one that we can successfully connect to */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue; /* Socket failed, try the next */

        /* Connect to the server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
            break; /* Success */
        close(clientfd); /* Connect failed, try another */  //line:netp:openclientfd:closefd
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* All connects failed */
        return -1;
    else    /* The last connect succeeded */
        return clientfd;
}
/* $end open_clientfd */

static void closeConnection(Connection *p) {
    debug("close connection %d", p -> dst);

    if (p -> src > 0) closeFD(p -> src);
    closeFD(p -> dst);
    free(p);
}

static int split(const char **src, char **dst, char delim, int forward) {
    for (; *(*src) && *(*src) != delim; ++(*src)) *(*dst)++ = *(*src);
    if (!*(*src)) return 0;
    *(*dst)++ = 0;
    if (forward) ++(*src);
    return 1;
}

static int parseURI(const char *buf, URI *uri) {
    static char line[MaxLine];
    char *now = line, *method, *host, *port, *request;

    method = now;
    if (!split(&buf, &now, ' ', 1)) return 0;
    debug("parse method: %s", method);

    const char *fit = HTTP;
    for (; *buf && *fit; ++buf, ++fit) {
        char c = *buf;
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
        if (c != *fit) return 0;
    }

    host = now;
    if (!split(&buf, &now, '/', 0)) return 0;

    for (port = host; *port && *port != ':'; ++port);
    if (*port) {
        *port++ = 0;
    } else {
        port = HTTPPort;
    }
    debug("parse host: %s", host);
    debug("parse port: %s", port);

    request = now;
    if (!split(&buf, &now, ' ', 1)) return 0;
    debug("parse request: %s", request);

    uri -> method = method;
    uri -> host = host;
    uri -> port = port;
    uri -> request = request;
    return 1;
}

static void clientHandler(int fd) {
    Connection *p = conn[fd];

    static char buf[BufSize];

    int n = read(fd, buf, BufSize);

    if (n <= 0) {
        debug("EOF or error detected from %d, close", fd);
        closeConnection(p);
    } else {
        debug("transfer %d bytes to %d", n, p -> dst);
        write(p -> dst, buf, n);
    }
}

static void writeDefaultHeader(int fd) {
    static const char *str[] = {
        "Connection: close\r\n",
        "Proxy-Connection: close\r\n",
    };
    int i;
    for (i = 0; i < 2; ++i) {
        write(fd, str[i], strlen(str[i]));
    }
}

static int handleURI(Connection *p, const char *line) {
    URI uri;
    if (!parseURI(line, &uri)) {
        warn("parse URI error for %d", p -> dst);
        return 0;
    }

    int clientfd = open_clientfd(uri.host, uri.port);
    if (clientfd < 0) {
        excp("open client %d failed", p -> dst);
        return 0;
    }
    p -> src = clientfd;
    conn[clientfd] = p;
    listenFD(clientfd, clientHandler);

    static char req[MaxLine];
    sprintf(req, "%s %s %s\r\n", uri.method, uri.request, HTTPVer);
    write(clientfd, req, strlen(req));
    sprintf(req, "Host: %s\r\n", uri.host);
    write(clientfd, req, strlen(req));
    writeDefaultHeader(clientfd);

    p -> state = header;

    return 1;
}

static int handleHeader(Connection *p, const char *buf) {
    static char line[MaxLine];
    static char req[MaxLine];
    char *now = line;

    if (!*buf || (*buf == '\r' && !*(buf + 1))) {
        debug("complete header from %d", p -> dst);
        debug("content length: %d", p -> content);
        write(p -> src, "\r\n", 2);
        p -> state = content;
        return 1;
    }

    char *header = now;
    if (!split(&buf, &now, ':', 1)) return 0;
    debug("parse header: %s", header);

    char *value = now;
    split(&buf, &now, '\r', 1);
    debug("parse value: %s", value);

    if (!strcmp(header, "Connection") || !strcmp(header, "Proxy-Connection")) return 1;
    if (!strcmp(header, "Content-Length")) {
        p -> content = atoi(value);
    }

    sprintf(req, "%s: %s\r\n", header, value);
    write(p -> src, req, strlen(req));

    return 1;
}

static void requestHandler(int fd) {
    Connection *p = conn[fd];
    require(p -> state != closed);
    int n = fillBuffer(&p -> buf);
    const char *line;
    int flag = 1;

    if (n <= 0) {
        warn("EOF or error detected from %d", fd);
        closeConnection(p);
        return;
    }

    while (flag && p -> state != content && (line = readLine(&p -> buf))) {
        if (p -> state == uri) {
            flag = handleURI(p, line);
        } else {
            flag = handleHeader(p, line);
        }
    }

    if (!flag) {
        warn("invalid request from %d", fd);
        closeConnection(p);
        return;
    }

    if (p -> state == content) {
        if (p -> content > 0) {
            if (p -> buf.remain > p -> content) p -> content = p -> buf.remain;
            write(p -> src, p -> buf.next, p -> buf.remain);
            p -> content -= p -> buf.remain;
            flushBuffer(&p -> buf);
        } else {
            debug("complete request from %d", p -> dst);
            unbindFD(p -> dst);
            p -> state = closed;
        }
    }
}

static Connection *openConnection(int fd) {
    Connection *ret = malloc(sizeof(Connection));
    ret -> dst = fd;
    ret -> src = -1;
    ret -> state = uri;
    ret -> content = 0;
    initBuffer(&ret -> buf, fd);
    listenFD(fd, requestHandler);
    conn[fd] = ret;
    return ret;
}

static void listenHandler(int fd) {
    struct sockaddr_storage clientaddr;
    socklen_t clientlen = sizeof(struct sockaddr_storage);
    int connfd = accept(fd, (struct sockaddr *)&clientaddr, &clientlen);
    openConnection(connfd);
}

void createServer(const char *port) {
    int listenfd = open_listenfd(port);
    listenFD(listenfd, listenHandler);
}
