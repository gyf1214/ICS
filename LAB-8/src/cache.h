#ifndef __CACHE
#define __CACHE

#include "config.h"
#include "proxy.h"

typedef struct cache_block_t{
    long long hashcode;
    char *buf;
    int size;
    struct cache_block_t *next;
}CacheBlock;

void initBlock(int, const URI *);
const char *cacheHandler(int, int *);
const char *queryBlock(const URI *, int *);

#endif
