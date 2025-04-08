#ifndef SRC_ADDRESSING_H_
#define SRC_ADDRESSING_H_

#include <stdint.h>

typedef struct Cpu Cpu;

// Use with #define ADDRESS_MODE_X(uppercase, lowercase, size)
#define ADDRESS_MODE_LIST                       \
    ADDRESS_MODE_X(ACCUMULATOR, accumulator, 1) \
    ADDRESS_MODE_X(ABSOLUTE, absolute, 3)       \
    ADDRESS_MODE_X(ABSOLUTE_X, absolute_x, 3)   \
    ADDRESS_MODE_X(ABSOLUTE_Y, absolute_y, 3)   \
    ADDRESS_MODE_X(IMMEDIATE, immediate, 2)     \
    ADDRESS_MODE_X(IMPLIED, implied, 1)         \
    ADDRESS_MODE_X(INDIRECT, indirect, 3)       \
    ADDRESS_MODE_X(INDIRECT_X, indirect_x, 2)   \
    ADDRESS_MODE_X(INDIRECT_Y, indirect_y, 2)   \
    ADDRESS_MODE_X(RELATIVE, relative, 2)       \
    ADDRESS_MODE_X(ZEROPAGE, zeropage, 2)       \
    ADDRESS_MODE_X(ZEROPAGE_X, zeropage_x, 2)   \
    ADDRESS_MODE_X(ZEROPAGE_Y, zeropage_y, 2)

typedef enum AddressMode
{
#define ADDRESS_MODE_X(uppercase, ...) AM_##uppercase,
    ADDRESS_MODE_LIST
#undef ADDRESS_MODE_X
} AddressMode;

typedef uint8_t(instruction_load_t)(Cpu *cpu, uint16_t operand);
typedef void(instruction_store_t)(Cpu *cpu, uint16_t operand, uint8_t value);
typedef uint16_t(instruction_address_t)(Cpu *cpu, uint16_t operand);

typedef struct
{
    instruction_load_t *const load;
    instruction_store_t *const store;
    instruction_address_t *const address;
    const uintptr_t size;
} Addressing;

#define ADDRESS_MODE_X(uppercase, lowercase, ...) \
    instruction_load_t cpu_load_##lowercase;      \
    instruction_store_t cpu_store_##lowercase;    \
    instruction_address_t cpu_address_##lowercase;
ADDRESS_MODE_LIST
#undef ADDRESS_MODE_X

static inline Addressing get_addressing(AddressMode mode)
{
    switch (mode)
    {
#define ADDRESS_MODE_X(uppercase, lowercase, inst_size) \
    case AM_##uppercase:                                \
        return (Addressing){                            \
            .load = cpu_load_##lowercase,               \
            .store = cpu_store_##lowercase,             \
            .address = cpu_address_##lowercase,         \
            .size = inst_size,                          \
        };
        ADDRESS_MODE_LIST
    }
#undef ADDRESS_MODE_X
}

#endif // SRC_ADDRESSING_H_
