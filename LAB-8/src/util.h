#ifndef __UTIL
#define __UTIL

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/errno.h>

#ifdef DEBUG
#define log(level, format, ...) fprintf(stderr, "[%s](%s: %d)\t" format "\n", level, __FILE__, __LINE__,##__VA_ARGS__)
#define debug(...)              log("debug", __VA_ARGS__)
#define require(x)              assert(x)
#else
#define log(level, format, ...) fprintf(stderr, "[%s] " format "\n", level,##__VA_ARGS__)
#define debug(...)
#define require(x)
#endif

#define warn(...)           log("warn", __VA_ARGS__)
#define excp(format, ...)   warn(format " %s(%d)",##__VA_ARGS__, strerror(errno), errno)
#define error(...)          log("error", __VA_ARGS__), exit(-1)
#define fatal(format, ...)  error(format " %s(%d)",##__VA_ARGS__, strerror(errno), errno)

#endif
