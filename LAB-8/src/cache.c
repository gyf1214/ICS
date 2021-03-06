#include <unistd.h>
#include <string.h>
#include "cache.h"
#include "util.h"

#define Mod     999999999989LL
#define Magic1  1373
#define Magic2  3533

static long long hashString(const char *str) {
    long long ans = 0;
    for (; *str; ++str) {
        ans = (ans * Magic1 + *str) % Mod;
    }
    return ans;
}

static long long hashURI(const URI *uri) {
    long long ans = 0;
    ans = (ans * Magic2 + hashString(uri -> method)) % Mod;
    ans = (ans * Magic2 + hashString(uri -> host)) % Mod;
    ans = (ans * Magic2 + hashString(uri -> port)) % Mod;
    ans = (ans * Magic2 + hashString(uri -> request)) % Mod;
    return ans;
}

static CacheBlock *head = NULL, *tail = NULL;
static CacheBlock *temp[MaxFD];
static int sizeCount = 0;

void initBlock(int fd, const URI *uri) {
    require(!temp[fd]);

    debug("init cache block for %d", fd);
    debug("request: %s %s:%s%s", uri -> method, uri -> host, uri -> port, uri -> request);

    temp[fd] = malloc(sizeof(CacheBlock));
    temp[fd] -> hashcode = hashURI(uri);
    temp[fd] -> size = 0;
    temp[fd] -> next = NULL;
    temp[fd] -> buf = malloc(ObjectSize);

    debug("block hashcode: %010llX", temp[fd] -> hashcode);
}

static void freeBlock(CacheBlock *block) {
    require(block);

    debug("drop block %010llX", block -> hashcode);
    if (block -> buf) free(block -> buf);
    free(block);
}

static void prepareBlock(int size) {
    size += sizeof(CacheBlock);
    while (size + sizeCount > CacheSize) {
        require(head);

        CacheBlock *now = head;
        debug("evict block %010llX", now -> hashcode);
        head = now -> next;
        sizeCount -= now -> size - sizeof(CacheBlock);
        freeBlock(now);
    }
    sizeCount += size;
}

static void insertTail(CacheBlock *block) {
    block -> next = NULL;
    if (!head) {
        head = tail = block;
    } else {
        tail -> next = block;
        tail = block;
    }
}

static void insertBlock(CacheBlock *block) {
    require(block);

    debug("insert block %010llX", block -> hashcode);
    block -> buf = realloc(block -> buf, block -> size);
    prepareBlock(block -> size);
    insertTail(block);
    debug("cache size: %d", sizeCount);
}

const char *cacheHandler(int fd, int *pn) {
    require(temp[fd]);
    int n;
    char *ret;

    if (temp[fd] -> size >= ObjectSize) {
        ret = temp[fd] -> buf;
        n = read(fd, ret, ObjectSize);
        if (temp[fd] -> size == ObjectSize && n > 0) {
            temp[fd] -> size = ObjectSize + 1;
            debug("ObjectSize exceed for %d", fd);
        }
    } else {
        ret = temp[fd] -> buf + temp[fd] -> size;
        n = read(fd, ret, ObjectSize - temp[fd] -> size);
        temp[fd] -> size += n;
    }

    if (n <= 0) {
        debug("error or EOF detected from %d", fd);
        if (!n && temp[fd] -> size <= ObjectSize) {
            insertBlock(temp[fd]);
        } else {
            freeBlock(temp[fd]);
        }
        temp[fd] = NULL;
        return NULL;
    } else {
        debug("read %d bytes from %d", n, fd);
        *pn = n;
        return ret;
    }
}

const char *queryBlock(const URI *uri, int *pn) {
    long long code = hashURI(uri);
    debug("find block %010llX", code);

    CacheBlock *p, *q;
    for (p = NULL, q = head; q; p = q, q = q -> next) {
        if (q -> hashcode == code) break;
    }

    if (!q) {
        debug("miss %010llX", code);
        return NULL;
    }

    debug("hit %010llX, size: %d", code, q -> size);
    *pn = q -> size;
    if (!p) {
        head = q -> next;
    } else {
        p -> next = q -> next;
    }
    if (q == tail) tail = p;
    insertTail(q);
    return q -> buf;
}
