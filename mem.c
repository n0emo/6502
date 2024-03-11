#include "mem.h"
#include <stdlib.h>

#define MEMORY_CAPACITY (64 * 1024)

void mem_init(Memory *mem)
{
    mem->data = calloc(sizeof(mem->data[0]) * MEMORY_CAPACITY, 1);
    mem->capacity = MEMORY_CAPACITY;
}
