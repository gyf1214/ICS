/*
 * MM using Segregated Free List
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
// #define DEBUG
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
#define ALIGN(size, n) (((size) + ((n) - 1)) & (~((n) - 1)))

/* start Tiny defines */

typedef char *ptr, **pptr;
typedef unsigned int u32, *p32;
typedef unsigned long u64, *p64;

#define PACK(size, used)        ((size) | (used))
#define TAG(p)                  (*((p32)(p) - 1))
#define SIZE(p)                 (TAG(p) & (-4))
#define USED(p)                 (TAG(p) & 1)
#define LUSED(p)                (TAG(p) & 2)
#define ETAG(p)                 (*(p32)((p) + SIZE(p)))
#define LTAG(p)                 (*((p32)(p) - 2))
#define LSIZE(p)                (LTAG(p) & (-4))
#define RIGHT(p)                ((p) + SIZE(p) + 8)
#define LEFT(p)                 ((p) - LSIZE(p) - 8)
#define NEXT(p)                 (*(p32)(p))
#define PREV(p)                 (*((p32)(p) + 1))
#define PTR(p)                  ((p) ? (base + (p)) : NULL)
#define OFF(p)                  ((p) ? ((p) - base) : 0)

#define CHUNKSIZE               16

#define MINSIZE                 8
#define LISTCNT                 12
#define LIST(i)                 ((ptr)((p32)(base) + (i) + 1))

#ifdef DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif

/* end Tiny defines */

/* strat Tiny globals */

/* base address of heap, and also the base address of seg list */
static ptr base;

/* end Tiny globals */

/* start Tiny functions */

/* keep USED(p) = LUSED(RIGHT(p)) */
static inline void updateLUsed(ptr p) {
    TAG(RIGHT(p)) &= -3;
    TAG(RIGHT(p)) |= (USED(p) << 1);
}

/* set TAG of a block both at head & foot */
static inline void setTag(ptr p, u32 tag) {
    TAG(p) = tag;
    if (!USED(p)) ETAG(p) = tag & (-4);
    updateLUsed(p);
}

/* set size, keep used & lused */
static inline void setSize(ptr p, u32 size) {
    u32 bits = TAG(p) & 3;
    size |= bits;
    TAG(p) = size;
    if (!USED(p)) ETAG(p) = size;
}

/* mark an unused block */
static inline void unsetUsed(ptr p) {
    TAG(p) &= -2;
    updateLUsed(p);
    ETAG(p) = SIZE(p);
}

/* find the seg index of size */
static inline u32 getIndex(u32 size) {
    ASSERT(size >= MINSIZE);

    u32 k = 0;
    for (size /= MINSIZE; size && k < LISTCNT; size /= 2) ++k;
    return k - 1;
}

/* align to 8 byte and al least MINSIZE */
static inline u32 align(u32 p) {
    p = p < MINSIZE + 4 ? MINSIZE : p - 4;
    return ALIGN(p, ALIGNMENT);
}

/* insert a free block into the seg list */
static void insertBlock(ptr p) {
    ASSERT(!USED(p));

    /* choose the list */
    u32 size = SIZE(p);
    u32 k = getIndex(size);

    // /* insert to head */
    // ptr q = LIST(k);

    /* insert so that the list is sorted */
    ptr q = LIST(k);
    for (; NEXT(q); q = PTR(NEXT(q))) {
        if (SIZE(PTR(NEXT(q))) >= size) break;
    }
    PREV(p) = OFF(q);
    NEXT(p) = NEXT(q);
    if (NEXT(q)) PREV(PTR(NEXT(q))) = OFF(p);
    NEXT(q) = OFF(p);
}

/* delete a free block from the seg list */
static void deleteBlock(ptr p) {
    ASSERT(!USED(p));

    NEXT(PTR(PREV(p))) = NEXT(p);
    if (NEXT(p)) PREV(PTR(NEXT(p))) = PREV(p);
}

/* merge p and RIGHT(p), remain USED & LUSED of p */
static ptr mergeBlock(ptr p) {
    ptr q = RIGHT(p);

    /* RIGHT(p) cannot be USED! */
    ASSERT(!USED(q));

    u32 size = SIZE(p) + SIZE(q) + 8;
    deleteBlock(q);
    setSize(p, size);
    updateLUsed(p);

    return p;
}

/* coalesce */
static ptr coalesceBlock(ptr p) {
    ASSERT(!USED(p));

    /* merge left */
    if (!LUSED(p)) p = mergeBlock(LEFT(p));
    /* merge right */
    if (!USED(RIGHT(p))) p = mergeBlock(p);

    return p;
}

/* allocate size from a free block, used set to 1 */
static ptr allocateBlock(ptr p, u32 size) {
    ASSERT(size >= MINSIZE);

    if (!USED(p)) deleteBlock(p);

    u32 fullSize = SIZE(p);
    u32 lused = LUSED(p);
    ASSERT(fullSize >= size);

    int remainder = fullSize - size - 8;
    if (remainder >= MINSIZE) {
        /* split */
        ptr q = p + size + 8;
        setTag(p, PACK(size, 1 | lused));
        setTag(q, PACK(remainder, 2));
        insertBlock(q);
    } else {
        /* simply gives the whole block */
        setTag(p, PACK(fullSize, 1 | lused));
    }

    return p;
}

/* extend heap and return the new block */
static ptr extendHeap(u32 size) {
    size = ALIGN(size + 8, CHUNKSIZE);

    /* increase heap size */
    ptr p = mem_sbrk(size);
    u32 lused = LUSED(p);
    setTag(p, PACK(size - 8, lused));
    /* epilogue */
    TAG(RIGHT(p)) = 1;
    /* insert into seg list */
    insertBlock(p);
    /* coalesce */
    return coalesceBlock(p);
}

/* find a free block with at least the required size */
static ptr findBlock(u32 size) {
    /* find block from the least seg */
    int k = getIndex(size);
    ptr p = NULL;
    for (; !p && k < LISTCNT; ++k) {
        for (p = PTR(NEXT(LIST(k))); p; p = PTR(NEXT(p))) {
            if (SIZE(p) >= size) break;
        }
    }

    if (p) {
        /* found */
        return p;
    } else {
        /* extend heap */
        return extendHeap(size);
    }
}

/* resize a block to size, returns NULL when fails */
static ptr resizeBlock(ptr p, u32 size) {
    int more = size - SIZE(p);

    /* if RIGHT(p) is not USED */
    ptr q = RIGHT(p);
    if (!USED(q)) {
        /* we can merge with right */
        more -= SIZE(q) + 8;
        p = mergeBlock(p);
    }

    /* split and successfully return if it's enough */
    if (more <= 0) return allocateBlock(p, size);

    /* if p is the most right block */
    if (!SIZE(RIGHT(p))) {
        /* we can extend the heap and merge the new area */
        extendHeap(more - 8);
        p = mergeBlock(p);
        return allocateBlock(p, size);
    }

    /* sadly fails */
    return NULL;
}

/* end Tiny functions */

/* start Tiny debug */

#ifdef DEBUG

static inline int aligned(ptr p) {
    return (u32)p == ALIGN((u32)p, 8);
}

static inline int inHeap(ptr p) {
    return p <= (ptr)mem_heap_hi() && p >= (ptr)mem_heap_lo();
}

/* check a general block */
static void checkBlock(ptr p) {
    ASSERT(inHeap(p));
    ASSERT(aligned(p));
    if (!USED(p)) {
        ASSERT(SIZE(p) == ETAG(p));
    }
    ASSERT(LUSED(RIGHT(p)) == (USED(p) << 1));
}

/* check free list */
static void checkList(int k) {
    for (ptr p = LIST(k), q = PTR(NEXT(p)); q; p = q, q = PTR(NEXT(q))) {
        ASSERT(PTR(NEXT(p)) == q);
        ASSERT(PTR(PREV(q)) == p);
        ASSERT(!USED(q));
        ASSERT(SIZE(q) / MINSIZE >= (1u << k));
        if (k < LISTCNT - 1) ASSERT(SIZE(q) / MINSIZE < (2u << k));
    }
}

#endif

/* end Tiny debug */

/*
 * mm_init - Called when a new trace starts.
 * CAUTION: You must reset all of your global pointers here.
 */
int mm_init(void) {
    /* allocate seg list */
    u32 headSize = LISTCNT * sizeof(ptr) + 16;
    base = mem_sbrk(headSize);
    if (base == (ptr) -1) return -1;

    /* head of the seg list */
    for (int i = 0; i < LISTCNT; ++i) NEXT(LIST(i)) = OFF((ptr)NULL);

    ptr p = base + headSize;
    /* prologue */
    LTAG(p) = 1;
    /* epilogue */
    TAG(p) = 3;

    return 0;
}

/*
 * malloc - Allocate a block by incrementing the brk pointer.
 *      Always allocate a block whose size is a multiple of the alignment.
 */
void *malloc(size_t _size) {
    /* trival */
    if (!_size) return NULL;

    u32 size = align(_size);
    ptr p = findBlock(size);
    return allocateBlock(p, size);
}

/*
 * free - We don't know how to free a block.  So we ignore this call.
 *      Computers have big memories; surely it won't be a problem.
 */
void free(void *_p) {
    ptr p = (ptr) _p;

    if (!p) return;

    /* unset USED */
    unsetUsed(p);
    insertBlock(p);
    /* coalesce */
    coalesceBlock(p);
}

/*
 * realloc - Change the size of the block by mallocing a new block,
 *      copying its data, and freeing the old block.  I'm too lazy
 *      to do better.
 */
void *realloc(void *old, size_t _size) {
    /* trival situation */
    if (!_size) {
        free(old);
        return NULL;
    }
    if (!old) return malloc(_size);

    ptr p = (ptr)old;
    u32 size = align(_size);

    /* try to resize the block */
    ptr q = resizeBlock(p, size);
    if (q) return q;

    /* if fails then resize brutely */
    q = malloc(size);
    memcpy(q, p, size);
    free(p);
    return q;
}

/*
 * calloc - Allocate the block and set it to zero.
 */
void *calloc(size_t nmemb, size_t size)
{
    size_t bytes = nmemb * size;
    void *newptr;

    newptr = malloc(bytes);
    memset(newptr, 0, bytes);

    return newptr;
}

/*
 * mm_checkheap - There are no bugs in my code, so I don't need to
 *      check, so nah! (But if I did, I could call this function using
 *      mm_checkheap(__LINE__) to identify the call site.)
 */
void mm_checkheap(int lineno){
#ifdef DEBUG
    ptr p = base + LISTCNT * sizeof(ptr) + 8;
    /* prologue */
    ASSERT(LTAG(p) == 1);
    ASSERT(LUSED(p));
    /* check blocks */
    for (; SIZE(p); p = RIGHT(p)) checkBlock(p);

    for (int k = 0; k < LISTCNT; ++k) checkList(k);
#endif
}
