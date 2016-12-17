#ifndef __UTIL
#define __UTIL

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef DEBUG
#define log(format, ...)    fprintf(stderr, "[%s: %d] " format "\n", __FILE__, __LINE__,##__VA_ARGS__)
#define debug(...)          log(__VA_ARGS__)
#define require(x)          assert(x)
#else
#define log(format, ...)    fprintf(stderr, format "\n",##__VA_ARGS__)
#define debug(...)
#define require(x)
#endif

#define warn(...)           log(__VA_ARGS__)
#define error(...)          log(__VA_ARGS__), exit(-1)

#endif
