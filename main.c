#include <stdlib.h>
#include <stdio.h>

#include "./mem_alloc.h"

int main() {
    void * break_addr = get_break();
    printf("start at: %p\n", break_addr);

    void * ptr1 = mem_alloc(33);
    printf("allocated ptr addr: %p\n", ptr1);

    void * ptr2 = mem_alloc(55);
    printf("allocated ptr addr: %p\n", ptr2);

    void * ptr3 = mem_alloc(17);
    printf("allocated ptr addr: %p\n", ptr3);

    void * ptr4 = mem_alloc(4);
    printf("allocated ptr addr: %p\n", ptr4);

    mem_free(ptr2);
    
    dump_heap();

    mem_free(ptr4);
    printf("Free ptr4\n");

    dump_heap();

    return 0;
}