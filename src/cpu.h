#ifndef INCLUDE_MOS6502_CPU_H_
#define INCLUDE_MOS6502_CPU_H_

#define CPU_RESET_VECTOR_1 0xFFFC
#define CPU_RESET_VECTOR_2 0xFFFD

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mem.h"

#define UNUSED(...) (void)(__VA_ARGS__)
#define UNUSED2(x, y) UNUSED(x), UNUSED(y)
#define UNUSED3(x, y, z) UNUSED(x), UNUSED(y), UNUSED(z)

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

void cpu_print(Cpu *cpu);
void cpu_init(Cpu *cpu, Memory *mem);
void cpu_reset(Cpu *cpu);
void cpu_execute(Cpu *cpu);
void cpu_set_z(Cpu *cpu, uint8_t value);
void cpu_set_n(Cpu *cpu, uint8_t value);
void cpu_set_zn(Cpu *cpu, uint8_t value);
uint8_t cpu_get_status(Cpu *cpu);
void cpu_set_status(Cpu *cpu, uint8_t status);
void cpu_push(Cpu *cpu, uint8_t value);
void cpu_push16(Cpu *cpu, uint16_t value);
uint8_t cpu_pull(Cpu *cpu);
uint16_t cpu_pull16(Cpu *cpu);

#endif // INCLUDE_MOS6502_CPU_H_
