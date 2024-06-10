#include <stdio.h>

#include "malloc.h"

int main() {
    void * ptr = malloc(sizeof(int));

    printf("ptr: %p\n", ptr);

    return 0;
}