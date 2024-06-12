#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define METADATA_SIZE sizeof(mem_block_metadata)

#define BLOCK_SIZE_UNIT 32

static void * heap_head = NULL;
static void * heap_tail = NULL;

typedef struct mem_block_metadata_t {
    void * prev;
    size_t size;
    bool free;
} mem_block_metadata;

// malloc implementation
void * mem_alloc(size_t size);
void mem_free(void * ptr);

// debug / test
void dump_ptr(void * ptr);
void dump_blk(mem_block_metadata * blk);
void dump_heap();

// brk / srbk wrapper
void * get_break();
void * trunc_heap(mem_block_metadata * blk);

// block merges
mem_block_metadata * merge_left(mem_block_metadata * blk);
void merge_right(mem_block_metadata * blk);

// block utils
mem_block_metadata * new_blk(size_t size);
mem_block_metadata * get_blk(void * ptr);
size_t get_block_size(size_t size);
mem_block_metadata * get_next_blk(mem_block_metadata * blk);


#endif // MEM_ALLOC_H