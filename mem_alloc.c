#include "mem_alloc.h"

void * mem_alloc(size_t size) {
    if(heap_head == NULL) {
        heap_head = get_break();
    }

    void * ptr = new_blk(size);

    return ptr;
}

void mem_free(void * ptr) {
    mem_block_metadata * blk = get_blk(ptr);
    blk->free = true;

    blk = merge_left(blk);
    merge_right(blk);

    if(blk == heap_tail) {
        heap_tail = trunc_heap(blk->prev);
        printf("Truncated heap\n");
    }
}

void * get_break() {
    void * current_break = sbrk(0);
    if(current_break == (void *)-1) {
        perror("sbrk call error");
        exit(EXIT_FAILURE);
    }

    return current_break;
}

void * trunc_heap(mem_block_metadata * blk) {
    if(brk(blk) == 0) return blk;

    perror("sbrk call error");
    exit(EXIT_FAILURE);

    return get_break();
}

mem_block_metadata * new_blk(size_t size) {
    size_t blk_size = get_block_size(size);
    mem_block_metadata * metadata = sbrk(METADATA_SIZE);

    metadata->size = blk_size;
    metadata->free = false;
    metadata->prev = heap_tail;
    heap_tail = metadata;

    return sbrk(blk_size);
}

mem_block_metadata * get_blk(void * ptr) {
    return ptr - (METADATA_SIZE / sizeof(*ptr));
}

size_t get_block_size(size_t size) {
    size_t delta = BLOCK_SIZE_UNIT - (size % BLOCK_SIZE_UNIT);
    return size + (delta ? delta : BLOCK_SIZE_UNIT);
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

void dump_ptr(void * ptr) {
    mem_block_metadata * blk = get_blk(ptr);
    dump_blk(blk);
}

void dump_blk(mem_block_metadata * blk) {
    printf("\n===== blk =====\n");
    printf("Addr: %p\n", blk);
    printf("Prev: %p\n", blk->prev);
    printf("Size: %lu\n", blk->size);
    printf("Free: %s\n", blk->free ? "true" : "false");
    printf("Data addr: %p\n", blk+1);
    printf("===== -*- =====\n");
}

void dump_heap() {
    mem_block_metadata * blk;
    for(blk = heap_head; blk <= heap_tail; blk = get_next_blk(blk)) {
        dump_blk(blk);
    }
}