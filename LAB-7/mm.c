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

typedef unsigned int u32;
typedef unsigned long u64;

#define PACK(size, used)        ((size) | (used))
#define TAG(p)                  (*((u32 *)(p) - 1))
#define SIZE(p)                 (TAG(p) & (-2))
#define USED(p)                 (TAG(p) & 1)
#define ETAG(p)                 (*(u32 *)((p) + SIZE(p)))
#define PTR(p)                  (char *)(base + (p))
#define OFF(p)                  (u32)((p) - base)
#define NEXT(p)                 (*(u32 *)(p))
#define PREV(p)                 (*(u32 *)((p) + 1))
#define RIGHT(p)                ((p) + SIZE(p) + 8)
#define LEFT(p)                 ((p) - SIZE((u32 *)(p) - 1) - 8)

#define CHUNKSIZE               (1 << 12)

/* end Tiny defines */

/* start Tiny globals */

static char *base;

/* end Tiny globals */

/* start Tiny functions */

static inline void *extendChunk(size_t size) {
    size = (size + CHUNKSIZE - 1) & (~(CHUNKSIZE - 1));
    return mem_sbrk(size);
}

static inline void setTag(char *p, u32 x) {
    TAG(p) = x;
    if (OFF(p) + SIZE(p) < mem_heapsize()) {
        ETAG(p) = x;
    }
}

/* end Tiny functions */

/*
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) {
    base = extendChunk(CHUNKSIZE);
    if (base == (void *)-1) return -1;
    NEXT(base) = 8;
    char *p = PTR(8);
    setTag(p, PACK(CHUNKSIZE - 8, 0));
    PREV(p) = NEXT(p) = 0;
    return 0;
}

/*
 * malloc
 */
void *malloc (size_t size) {
    return NULL;
}

/*
 * free
 */
void free (void *ptr) {
    if(!ptr) return;
}

/*
 * realloc - you may want to look at mm-naive.c
 */
void *realloc(void *oldptr, size_t size) {
    return NULL;
}

/*
 * calloc - you may want to look at mm-naive.c
 * This function is not tested by mdriver, but it is
 * needed to run the traces.
 */
void *calloc (size_t nmemb, size_t size) {
    return NULL;
}


/*
 * Return whether the pointer is in the heap.
 * May be useful for debugging.
 */
static int in_heap(const void *p) {
    return p <= mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Return whether the pointer is aligned.
 * May be useful for debugging.
 */
static int aligned(const void *p) {
    return (size_t)ALIGN(p) == (size_t)p;
}

/*
 * mm_checkheap
 */
void mm_checkheap(int lineno) {
}
