/*
 * mm.c - I began with the solution directly from csapp.cs.cmu.edu which gave a score of 45/100.
 *
 * Their solution:
 * Simple, 32-bit and 64-bit clean allocator based on implicit free
 * lists, first fit placement, and boundary tag coalescing, as described
 * in the CS:APP2e text. Blocks must be aligned to doubleword (8 byte)
 * boundaries. Minimum block size is 16 bytes.
 *
 * My altered solution:
 * 32-bit and 64-bit allocator based on explicit free lists, best fit placement,
 * and boundary tag coalescing. Blocks are aligned to doubleword (8 byte) boundaries.
 * The minimum block size taken is 4 words (16 bytes).
 * First declare our macros, constants, global variables, and function prototypes.
 * In mm_init, initialize the malloc package by creating the initial empty heap and
 * extending it with the minimum free block size.
 * In mm_malloc, allocate a block with at least size bytes of the payload.
 * Adjust the block size to include overhead and alignment requirements, then
 * search the free list for a fit. If no fit is found, get more memory and place the block.
 * In mm_free, free a block and then coalesce.
 * In mm_realloc, reallocate the block ptr to a block with at least size bytes of payload.
 * If size is zero, free block ptr and return NULL.
 * If size is greater than current block size + next block is empty, combine
 * current block and next block to resize without calling mm_malloc.
 * If nothing can be done, allocate new block with mm_malloc.
 * Then the contents of the old block ptr are copied to new block.
 * In coalesce, use boundary tag coalescing to remove and insert free block pointers
 * to the free list and return ptr to coalesced block. Cover the cases in which
 * only the next block is free, only the previous block is free, or both blocks are free.
 * In extend_heap, extend the heap with free block and return its block pointer. Set the
 * alignment and minimum block size and coalesce.
 * In find_fit, find best fit for a block with asize bytes from free list, extend the heap in
 * certain cases, and return said block's address.
 * In place, place block of asize bytes at start of free block bp and split if remainder
 * would be at least minimum block size.
 * Lastly, in free_list_insert and free_list_remove, insert and remove the free block pointer
 * in the free list respectivley.
 * New solution's score is 86/100.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "casillas",
    /* First member's full name */
    "Hunter Casillas",
    /* First member's email address */
    "huntercasillas@me.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* Macros and constants from the book */
#define WSIZE 4 // Word and header/footer size (bytes)
#define DSIZE 8 // Double word size (bytes)
#define CHUNKSIZE (1<<12) // Extend heap by this amount (bytes)
#define MAX(x, y) ((x) > (y)? (x) : (y)) // Maximum value

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p. */
#define GET(p) (*(uintptr_t *)(p))
#define PUT(p, val) (*(uintptr_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~(DSIZE - 1))
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((void *)(bp) - WSIZE)
#define FTRP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLK(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLK(bp) ((void *)(bp) - GET_SIZE((void *)(bp) - DSIZE))
/* End of macros from book */

/* Given ptr, get the next and previous ptr in the list */
#define GET_NEXT(bp) (*(char **)(bp + WSIZE))
#define GET_PREV(bp) (*(char **)(bp))

/* Put ptr in next and previous elements of the list */
#define SET_NEXT(bp, qp) (GET_NEXT(bp) = qp)
#define SET_PREV(bp, qp) (GET_PREV(bp) = qp)

/* Global variables */
static char *heap_listp = 0;
static char *begin_free_list = 0;

/* Function prototypes */
static void *extend_heap(size_t words);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void place(void *bp, size_t asize);
static void free_list_insert(void *bp);
static void free_list_remove(void *bp);
//static void mm_check(void *bp);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    /* Create the initial empty heap. */
    if ((heap_listp = mem_sbrk(8 * WSIZE)) == NULL) {
        return -1;
    }
    
    /* Set up directly from book */
    PUT(heap_listp, 0);                            /* Alignment padding */
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));     /* Epilogue header */
    begin_free_list = heap_listp + 2 * WSIZE;
    
    /* Extend the heap with minimum free block size */
    if (extend_heap(4) == NULL) {
        return -1;
    }
    return 0;
}

/* 
 * mm_malloc - Allocate a block with at least size bytes of payload.
 */
void *mm_malloc(size_t size) {
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    void *bp;
    
    /* Ignore spurious requests. */
    if (size == 0) {
        return (NULL);
    }
    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE) {
        asize = 2 * DSIZE;
    } else {
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);
    }
    /* Search the free list for a fit. */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return (bp);
    }
    /* No fit found.  Get more memory and place the block. */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL) {
        return (NULL);
    }
    place(bp, asize);
    return (bp);
}

/*
 * mm_free - Free a block.
 */
void mm_free(void *bp) {
    size_t size;
    /* Ignore spurious requests. */
    if (bp == NULL) {
        return;
    }
    /* Free block, then coalesce. */
    size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

/*
 * mm_realloc - Reallocate the block ptr to a block with at least size bytes of payload.
 *              If size is zero, free block ptr and return NULL.
 *              If size is greater than current block size + next block is empty,
 *              combine current block and next block to resize without calling mm_malloc.
 *              If nothing can be done, allocate new block with mm_malloc.
 *              Contents of the old block ptr are copied to new block.
 */
void *mm_realloc(void *bp, size_t size) {
    if ((int)size < 0) {
        return NULL;
    /* If size is zero, free block ptr and return NULL. */
    } else if ((int)size == 0) {
        mm_free(bp);
        return NULL;
    } else if (size > 0) {
        /* Oldsize set */
        size_t oldsize = GET_SIZE(HDRP(bp));
        /* Newsize set to include space for header and footer */
        size_t newsize = size + 2 * WSIZE;
        
        /* If newsize < oldsize */
        if (newsize <= oldsize) {
            return bp;
        }
        /* If newsize > oldsize */
        else {
            size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLK(bp)));
            size_t csize;
            if (!next_alloc && ((csize = oldsize + GET_SIZE(HDRP(NEXT_BLK(bp))))) >= newsize) {
                free_list_remove(NEXT_BLK(bp));
                PUT(HDRP(bp), PACK(csize, 1));
                PUT(FTRP(bp), PACK(csize, 1));
                return bp;
            }
            else {
                void *new_ptr = mm_malloc(newsize);
                place(new_ptr, newsize);
                memcpy(new_ptr, bp, newsize);
                mm_free(bp);
                return new_ptr;
            }
        }
    } else {
        return NULL;
    }
}

/*
 * coalesce - Boundary tag coalescing. Remove and insert free block pointers to the free list
 *            and return ptr to coalesced block.
 */
static void *coalesce(void *bp) {
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLK(bp)));
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLK(bp))) || PREV_BLK(bp) == bp;
    size_t size = GET_SIZE(HDRP(bp));
    
    /* Case in which only the next block is free */
    if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLK(bp)));
        free_list_remove(NEXT_BLK(bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    
    /* Case in which only the previous block is free */
    else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLK(bp)));
        bp = PREV_BLK(bp);
        free_list_remove(bp);
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    
    /* Case in which both blocks are free */
    else if (!prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLK(bp))) + GET_SIZE(HDRP(NEXT_BLK(bp)));
        free_list_remove(PREV_BLK(bp));
        free_list_remove(NEXT_BLK(bp));
        bp = PREV_BLK(bp);
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    
    /* Insert into free list and return */
    free_list_insert(bp);
    return bp;
}

/*
 * extend_heap - Extend heap with free block and return its block pointer.
 */
static void *extend_heap(size_t words) {
    char *bp;
    size_t size;
    
    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    
    /* Set size to minimum block size of 4 words (16 bytes) */
    if (size < 16) {
        size = 16;
    }

    if ((int)(bp = mem_sbrk(size)) == -1) {
        return NULL;
    }
    
    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */
    PUT(HDRP(NEXT_BLK(bp)), PACK(0, 1)); /* New epilogue header */
    
    /* Coalesce */
    return coalesce(bp);
}

/*
 * find_fit - Find a fit for a block with asize bytes from free list,
 *            extend the heap in certain cases, and return said block's address.
 */
static void *find_fit(size_t asize) {
    void *bp;
    static int prev_size = 0;
    static int count = 0;
    
    if (prev_size == (int)asize) {
        if (count>30) {
            int extendsize = MAX(asize, 4 * WSIZE);
            bp = extend_heap(extendsize/4);
            return bp;
        } else {
            count++;
        }
    } else {
        count = 0;
    }
    for (bp = begin_free_list; GET_ALLOC(HDRP(bp)) == 0; bp = GET_NEXT(bp)) {
        if (asize <= (size_t)GET_SIZE(HDRP(bp))) {
            prev_size = asize;
            return bp;
        }
    }
    return NULL;
}

/*
 * place - Place block of asize bytes at start of free block bp
 *         and split if remainder would be at least minimum block size.
 */
static void place(void *bp, size_t asize) {
    size_t csize = GET_SIZE(HDRP(bp));
    
    if ((csize - asize) >= 4 * WSIZE) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        free_list_remove(bp);
        bp = NEXT_BLK(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
        coalesce(bp);
    } else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
        free_list_remove(bp);
    }
}

/*
 * free_list_insert - Inserts the free block pointer in the free list.
 */
static void free_list_insert(void *bp) {
    SET_NEXT(bp, begin_free_list);
    SET_PREV(begin_free_list, bp);
    SET_PREV(bp, NULL);
    begin_free_list = bp;
}

/*
 * free_list_remove - Removes the free block pointer in the free list.
 */
static void free_list_remove(void *bp) {
    if (GET_PREV(bp)) {
        SET_NEXT(GET_PREV(bp), GET_NEXT(bp));
    } else {
        begin_free_list = GET_NEXT(bp);
    }
    SET_PREV(GET_NEXT(bp), GET_PREV(bp));
}

/*
 * mm_check - We don't check anything right now.
 *
static void mm_check(void *bp) {
    No implementation of mm_check, minus 5 points.
} */

