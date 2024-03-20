#ifndef INCLUDE_MOS6502_MEM_H_
#define INCLUDE_MOS6502_MEM_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t *data;
    uint16_t begin_address; // TODO: Mirrorring
    uint16_t end_address;
    bool readonly;
} Device;

typedef struct
{
    Device *devices;
    size_t dev_count;
    size_t dev_capacity;
} Memory;

void mem_append(Memory *mem, Device dev);

void mem_init(Memory *mem);
uint8_t mem_read(Memory *mem, uint16_t address);
uint16_t mem_read16(Memory *mem, uint16_t address);
void mem_write(Memory *mem, uint16_t address, uint8_t value);
void mem_write_force(Memory *mem, uint16_t address, uint8_t value);
void mem_write16(Memory *mem, uint16_t address, uint8_t value);

#endif // INCLUDE_MOS6502_MEM_H_
