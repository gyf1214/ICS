#ifndef __CONFIG
#define __CONFIG

#include <sys/select.h>

#define MaxFD       FD_SETSIZE
#define ListenQ     MaxFD / 2
#define BufSize     8192
#define MaxLine     8192
#define Timeout     10
#define HTTP        "http://"
#define HTTPPort    "80"
#define HTTPVer     "HTTP/1.0"
// #define stdlog      logFile
// #define logPath     "proxy.log"

#endif
