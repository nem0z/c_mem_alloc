#include "mem_alloc.h"
#include <string.h>

static void * heap_head = NULL;
static void * heap_tail = NULL;

void * wsbrk(intptr_t increment) {
    void * prev_addr = sbrk(increment);
    if(prev_addr == (void *)(-1)) {
        perror("call sbrk error: ");
        exit(EXIT_FAILURE);
    }

    return prev_addr;
}

void wbrk(char * addr) {
    if(brk(addr) == (void *)(-1)) {
        perror("call brk error: ");
        exit(EXIT_FAILURE);
    }
}

void * mem_alloc(size_t size) {
    if(heap_head == NULL) {
        heap_head = get_break();
    }

    size_t blk_size = get_next_pow2(size);
    mem_block_metadata * blk = find_free_blk(blk_size);
    if(blk == NULL) return new_blk(size)+1;
        
    blk->free = false;
    blk->used = size;

    if(blk->size >= size + BLOCK_MIN_SIZE) {
        split_blk(blk, blk_size);
    }

    return blk+1;
}

void mem_free(void * ptr) {
    mem_block_metadata * blk = get_blk(ptr);
    blk->free = true;
    blk->used = 0;

    blk = merge_left(blk);
    merge_right(blk);

    if(blk == heap_tail) {
        heap_tail = ((mem_block_metadata *)trunc_heap(blk))->prev;
        printf("Truncated heap\n");
    }
}

void * mem_realloc(void * ptr, size_t new_size) {
    mem_block_metadata * blk = get_blk(ptr);
    if(blk->size >= new_size) return ptr;

    void * new_ptr = mem_alloc(new_size);
    mem_free(ptr);

    return new_ptr;
}

void * mem_calloc(size_t mb_count, size_t mb_size) {
    size_t size = mb_count * mb_size;
    void * ptr = mem_alloc(size);

    memset(&ptr, 0, mb_size);
    return ptr;
}

mem_block_metadata * find_free_blk(size_t size) {
    mem_block_metadata * blk;
    for(blk = heap_head; (void *)heap_tail > (void *)blk; blk = get_next_blk(blk)) {
        if(blk->free && blk->size >= size) return blk;
    }

    return NULL;
}

void * get_break() {
    return wsbrk(0);
}

void * trunc_heap(mem_block_metadata * blk) {
    wbrk(blk);
    return get_break();
}

mem_block_metadata * new_blk(size_t size) {
    size_t blk_size = get_next_pow2(size);
    mem_block_metadata * metadata = wsbrk(METADATA_SIZE);

    metadata->size = blk_size;
    metadata->used = size;
    metadata->free = false;
    metadata->prev = heap_tail;
    heap_tail = metadata;

    wsbrk(blk_size)
    return metadata;
}

mem_block_metadata * get_blk(void * ptr) {
    return ptr - (METADATA_SIZE / sizeof(*ptr));
}

size_t get_next_pow2(size_t n) {
  n = n - 1;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  return n + 1;
}

mem_block_metadata * get_next_blk(mem_block_metadata * blk) {
    void * neutral_blk_ptr =  (void *) blk;
    mem_block_metadata * tmp = (mem_block_metadata *) (neutral_blk_ptr + (blk->size));
    return tmp + 1;
}

mem_block_metadata * merge_left(mem_block_metadata * blk) {
    mem_block_metadata * prev_blk = blk->prev;
    if(!prev_blk->free) return blk;

    prev_blk->size = prev_blk->size + blk->size + METADATA_SIZE;
    mem_block_metadata * next_blk = get_next_blk(blk);
    next_blk->prev = prev_blk;

    return prev_blk;
}

void merge_right(mem_block_metadata * blk) {
    mem_block_metadata * next_blk = get_next_blk(blk);
    if(!next_blk->free) return;

    blk->size = blk->size + next_blk->size + METADATA_SIZE;
    
    mem_block_metadata *  next_next_blk = get_next_blk(next_blk);
    next_next_blk->prev = blk;

    if(next_next_blk == heap_tail) heap_tail = blk;
}

void split_blk(mem_block_metadata * blk, size_t blk_size) {
    size_t prev_size = blk->size;
    blk->size = blk_size;

    mem_block_metadata * next = get_next_blk(blk);
    next->size = prev_size - blk_size;
    next->used = 0;
    next->free = true;
    next->prev = blk;

    mem_block_metadata * next_next = get_next_blk(next);
    next_next->prev = next;
}

void dump_ptr(void * ptr) {
    mem_block_metadata * blk = get_blk(ptr);
    dump_blk(blk);
}

void dump_blk(mem_block_metadata * blk) {
    printf("\n===== blk =====\n");
    printf("Addr: %p\n", blk);
    printf("Prev: %p\n", blk->prev);
    printf("Size: %lu\n", blk->size);
    printf("Used: %lu\n", blk->used);
    printf("Free: %s\n", blk->free ? "true" : "false");
    printf("Data addr: %p\n", blk+1);
    printf("===== -*- =====\n");
}

void dump_heap() {
    mem_block_metadata * blk;
    for(blk = heap_head; (void *)heap_tail > (void *)blk ; blk = get_next_blk(blk)) {
        dump_blk(blk);
    }
}