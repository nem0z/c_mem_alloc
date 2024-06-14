#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define METADATA_SIZE sizeof(mem_block_metadata)
// const static size_t METADATA_SIZE = sizeof(mem_block_metadata);

#define BLOCK_MIN_SIZE (32 + METADATA_SIZE)

typedef struct mem_block_metadata_t {
    void * prev;
    size_t size;
    size_t used;
    bool free;
} mem_block_metadata;

// malloc implementation
void * mem_alloc(size_t size);
void mem_free(void * ptr);
void * mem_realloc(void * ptr, size_t new_size);
void * mem_calloc(size_t mb_count, size_t mb_size);

// debug / test
void dump_ptr(void * ptr);
void dump_blk(mem_block_metadata * blk);
void dump_heap();

// brk / srbk wrapper
void * wsbrk(intptr_t Increment);
void * wbrk(char *EndDataSegment);
void * get_break();
void * trunc_heap(mem_block_metadata * blk);

// block merges
mem_block_metadata * merge_left(mem_block_metadata * blk);
void merge_right(mem_block_metadata * blk);

// block splits
void split_blk(mem_block_metadata * blk, size_t blk_size);

// block utils
mem_block_metadata * new_blk(size_t size);
mem_block_metadata * get_blk(void * ptr);
mem_block_metadata * get_next_blk(mem_block_metadata * blk);
mem_block_metadata * find_free_blk(size_t size);

// block size
size_t get_block_size(size_t size);
size_t get_next_pow2(size_t n);


#endif // MEM_ALLOC_H