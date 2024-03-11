#ifndef INCLUDE_MOS6502_MEM_H_
#define INCLUDE_MOS6502_MEM_H_

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t *data;
    size_t capacity;
} Memory;

void mem_init(Memory *mem);

#endif // INCLUDE_MOS6502_MEM_H_
