#include <stdlib.h>
#include <stdio.h>

#include "./mem_alloc.h"

int main() {
    void * break_addr = get_break();
    printf("start at: %p\n", break_addr);

    void * ptr1 = mem_alloc(33);
    printf("allocated ptr addr: %p\n", ptr1);

    void * ptr2 = mem_alloc(8);
    printf("allocated ptr addr: %p\n", ptr2);

    void * ptr3 = mem_alloc(17);
    printf("allocated ptr addr: %p\n", ptr3);

    void * ptr4 = mem_alloc(4);
    printf("allocated ptr addr: %p\n", ptr4);
    
    dump_heap();

    mem_free(ptr4);
    printf("Free ptr4\n");

    printf("%p\n", get_break());

    void * ptr5 = mem_alloc(98);
    printf("allocated ptr addr: %p\n", ptr5);

    dump_heap();

    ptr5 = mem_realloc(ptr5, 112);
    printf("allocated ptr addr: %p\n", ptr5);

    ptr5 = mem_realloc(ptr5, 156);
    printf("allocated ptr addr: %p\n", ptr5);

    void * ptr6 = mem_alloc(48);
    printf("allocated ptr addr: %p\n", ptr6);

    dump_heap();

    return 0;
}