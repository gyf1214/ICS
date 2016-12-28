# HW 12 #

## 11.6 ##

### A ###

See `tiny-11.6.c`

### B ###

Response below:

    GET / HTTP/1.1
    Host: localhost:8888
    Connection: keep-alive
    Upgrade-Insecure-Requests: 1
    User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.95 Safari/537.36
    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
    Accept-Encoding: gzip, deflate, sdch, br
    Accept-Language: en-US,en;q=0.8,zh-CN;q=0.6,zh;q=0.4,ja;q=0.2

### C ###

HTTP 1.1

### D ###

- Host: the Internet host and port number of the resource being requested, as obtained from the original URI given by the user or referring resource

- Connection: The Connection general-header field allows the sender to specify options that are desired for that particular connection

- Upgrade-Insecure-Requests: Tells the server that this user agent supports the `upgrade-insecure-requests` mechanism (upgrade all HTTP links in HTTPS pages to HTTPS automatically)

- User-Agent: The User-Agent request-header field contains information about the user agent originating the request.

- Accept: The Accept request-header field can be used to specify certain media types which are acceptable for the response

- Accept-Encoding: The Accept-Encoding request-header field is similar to Accept, but restricts the content-codings that are acceptable in the response

- Accept-Language: The Accept-Language request-header field is similar to Accept, but restricts the set of natural languages that are preferred as a response to the request

## 11.8 ##

See `tiny-11.8.c`

## 12.16 ##

See `hello.c`

## 12.17 ##

- A: Because the main thread calls `exit` after the `pthread_create`, causing the while process to terminate. So the new thread won't print the message

- B: `pthread_join` or `pthread_exit`

## 12.22 ##

See `select.c`
