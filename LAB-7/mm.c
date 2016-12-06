/*
 * mm.c
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)

/* start Tiny defines */

typedef char *ptr;
typedef unsigned int u32;
typedef unsigned long u64;

#define PACK(size, used)        ((size) | (used))
#define TAG(p)                  (*((u32 *)(p) - 1))
#define SIZE(p)                 (TAG(p) & (-2))
#define USED(p)                 (TAG(p) & 1)
#define ETAG(p)                 (*(u32 *)((p) + SIZE(p)))
#define PTR(p)                  (ptr)(base + (p))
#define OFF(p)                  (u32)((p) - base)
#define NEXT(p)                 (*(u32 *)(p))
#define PREV(p)                 (*((u32 *)(p) + 1))
#define RIGHT(p)                ((p) + SIZE(p) + 8)
#define LEFT(p)                 ((p) - SIZE((u32 *)(p) - 1) - 8)

#define CHUNKSIZE               (1 << 12)

/* end Tiny defines */

/* start Tiny globals */

static ptr base;
static ptr end;

/* end Tiny globals */

/* start Tiny functions */

static inline void setTag(ptr p, u32 x) {
    TAG(p) = x;
    ETAG(p) = x;
}

static inline void deleteBlock(ptr p) {
    NEXT(PTR(PREV(p))) = NEXT(p);
    if (NEXT(p)) PREV(PTR(NEXT(p))) = PREV(p);
}

static inline void pushBlock(ptr p) {
    if (NEXT(base)) {
        ptr q = PTR(NEXT(base));
        PREV(q) = OFF(p);
        NEXT(p) = OFF(q);
    } else {
        NEXT(p) = 0;
    }
    PREV(p) = 0;
    NEXT(base) = OFF(p);
}

static inline ptr allocateBlock(ptr p, size_t size) {
    u32 capcity = SIZE(p);
    if (size + 8 < capcity) {
        ptr q = p + size + 8;
        NEXT(PTR(PREV(p))) = OFF(q);
        if (NEXT(p)) PREV(PTR(NEXT(p))) = OFF(q);
        PREV(q) = PREV(p);
        NEXT(q) = NEXT(p);
        setTag(q, PACK(capcity - size - 8, 0));
        setTag(p, PACK(size, 1));
    } else {
        deleteBlock(p);
        TAG(p) |= 1;
        ETAG(p) |= 1;
    }
    return p;
}

static inline ptr coalesceBlock(ptr p) {
    ptr q;
    if (p > base + 8) {
        q = LEFT(p);
        if (!USED(q)) {
            deleteBlock(q);
            setTag(q, PACK(SIZE(p) + SIZE(q) + 8, 0));
            p = q;
        }
    }
    q = RIGHT(p);
    if (q < end && !USED(q)) {
        deleteBlock(q);
        setTag(p, PACK(SIZE(p) + SIZE(q) + 8, 0));
    }
    return p;
}

static inline ptr extendChunk(size_t size) {
    size = (size + 8 + CHUNKSIZE - 1) & (~(CHUNKSIZE - 1));
    ptr p = mem_sbrk(size);
    setTag(p, PACK(size - 8, 0));
    end = p + size;
    p = coalesceBlock(p);
    pushBlock(p);
    return p;
}

static inline ptr findBlock(size_t size) {
    ptr q = NULL;
    for (ptr p = base; NEXT(p); p = q) {
        q = PTR(NEXT(p));
        if (SIZE(q) >= size) {
            break;
        }
    }
    if (!q || SIZE(q) < size) {
        q = extendChunk(size);
    }
    return q;
}

/*
 * Return whether the pointer is in the heap.
 * May be useful for debugging.
 */
static inline int in_heap(const void *p) {
    return p <= mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Return whether the pointer is aligned.
 * May be useful for debugging.
 */
static inline int aligned(const void *p) {
    return (size_t)ALIGN(p) == (size_t)p;
}

static inline void checkBlock(ptr p) {
#ifdef DEBUG
    assert(aligned(p));
    assert(in_heap(p));
    assert(TAG(p) == ETAG(p));
    if (!USED(p)) {
        if (PREV(p)) {
            assert(NEXT(PTR(PREV(p))) == OFF(p));
        } else {
            assert(NEXT(base) == OFF(p));
        }
        if (NEXT(p)) {
            assert(PREV(PTR(NEXT(p))) == OFF(p));
        } else {
        }
    }
#endif
}

/* end Tiny functions */

/*
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) {
    base = mem_sbrk(CHUNKSIZE);
    end = base + CHUNKSIZE;
    if (base == (void *)-1) return -1;
    NEXT(base) = 8;
    ptr p = PTR(8);
    setTag(p, PACK(CHUNKSIZE - 16, 0));
    PREV(p) = NEXT(p) = 0;
    return 0;
}

/*
 * malloc
 */
void *malloc (size_t size) {
    size = ALIGN(size);
    ptr p = findBlock(size);
    return allocateBlock(p, size);
}

/*
 * free
 */
void free (void *_p) {
    ptr p = (ptr) _p;
    if(!p) return;
    TAG(p) &= -2;
    ETAG(p) &= -2;
    p = coalesceBlock(p);
    pushBlock(p);
}

/*
 * realloc - you may want to look at mm-naive.c
 */
void *realloc(void *oldptr, size_t size) {
    if (!oldptr) return malloc(size);
    size = ALIGN(size);
    ptr p = (ptr) oldptr;
    u32 capcity = SIZE(p);
    if (size + 8 < capcity) {
        ptr q = p + size + 8;
        setTag(p, PACK(size, 1));
        setTag(q, PACK(capcity - size - 8, 0));
        pushBlock(q);
        return p;
    } else if (size <= SIZE(p)) {
        return p;
    } else {
        u32 more = size - capcity - 8;
        ptr q = RIGHT(p);
        if (q >= end) {
            q = extendChunk(more);
        } else {
            if (USED(q)) {
                q = NULL;
            } else {
                u32 new_cap = capcity + SIZE(q) + 8;
                if (new_cap < size) {
                    if (RIGHT(q) >= end) {
                        q = extendChunk(size - new_cap);
                    } else {
                        q = NULL;
                    }
                }
            }
        }
        if (q) {
            allocateBlock(q, more);
            setTag(p, PACK(capcity + SIZE(q) + 8, 1));
            return p;
        } else {
            q = malloc(size);
            memcpy(q, p, capcity);
            free(p);
            return q;
        }
    }
}

/*
 * calloc - you may want to look at mm-naive.c
 * This function is not tested by mdriver, but it is
 * needed to run the traces.
 */
void *calloc (size_t nmemb, size_t size) {
    void *ret = malloc(nmemb * size);
    memset(ret, 0, nmemb * size);
    return ret;
}

/*
 * mm_checkheap
 */
void mm_checkheap(int lineno) {
    for (ptr p = base + 8; p < end; p = RIGHT(p)) {
        checkBlock(p);
    }
}
