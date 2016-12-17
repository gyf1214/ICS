#ifndef __CONFIG
#define __CONFIG

#include <sys/select.h>

#define MaxFD       FD_SETSIZE
#define ListenQ     MaxFD / 2
#define BufSize     8192
#define MaxLine     BufSize
#define Timeout     10

#endif
