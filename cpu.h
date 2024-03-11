#ifndef INCLUDE_MOS6502_CPU_H_
#define INCLUDE_MOS6502_CPU_H_

#define CPU_RESET_VECTOR_1 0xFFFC
#define CPU_RESET_VECTOR_2 0xFFFD

#include "mem.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Cpu
{
    uint16_t PC; // program counter
    uint8_t A;   // accumulator
    uint8_t X;   // X register
    uint8_t Y;   // Y register
    uint8_t SP;  // stack pointer

    bool N; // negative
    bool V; // overflow
    bool U; // unused
    bool B; // break (not a real flag)
    bool D; // decimal
    bool I; // interrupt
    bool Z; // zero
    bool C; // carry

    Memory *mem;
    uint64_t cycles;
} Cpu;

typedef enum AddressMode
{
    AM_ACCUMULATOR,
    AM_ABSOLUTE,
    AM_ABSOLUTE_X,
    AM_ABSOLUTE_Y,
    AM_IMMEDIATE,
    AM_IMPLIED,
    AM_INDIRECT,
    AM_INDIRECT_X,
    AM_INDIRECT_Y,
    AM_RELATIVE,
    AM_ZEROPAGE,
    AM_ZEROPAGE_X,
    AM_ZEROPAGE_Y

} AddressMode;

typedef void(instruction_func_t)(Cpu *, AddressMode, uint8_t *);

typedef struct Instruction
{
    const char *name;
    instruction_func_t *func;
    AddressMode address_mode;
    uint64_t cycles;
    size_t size;
} Instruction;

void cpu_print(Cpu *cpu);
void cpu_init(Cpu *cpu, Memory *mem);
void cpu_reset(Cpu *cpu);
void cpu_execute(Cpu *cpu);
uint8_t cpu_load(Cpu *cpu, AddressMode mode, uint8_t *data);
void cpu_store(Cpu *cpu, uint8_t value, AddressMode mode, uint8_t *data);
void cpu_set_z(Cpu *cpu, uint8_t value);
void cpu_set_n(Cpu *cpu, uint8_t value);

instruction_func_t cpu_adc;
instruction_func_t cpu_lda;
instruction_func_t cpu_sta;
instruction_func_t cpu_tax;
instruction_func_t cpu_inx;
instruction_func_t cpu_brk;

bool u8sign(uint8_t value);

#endif // INCLUDE_MOS6502_CPU_H_
