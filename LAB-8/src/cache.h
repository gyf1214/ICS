#ifndef __CACHE
#define __CACHE

#include "config.h"
#include "proxy.h"

#define Mod     23333333333LL
#define Magic   137

typedef struct cache_block_t{
    long long hashcode;
    char *buf;
    struct cache_block_t *next;
}CacheBlock;

void initBlock(int);
void addBlock(int, char *, int);
void pushBlock(int);
const char *queryBlock(URI *, int *);

#endif
