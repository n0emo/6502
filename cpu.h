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

typedef enum InstructionType
{
    IT_ADC,
    IT_ALR, // Illegal
    IT_ANC, // Illegal
    IT_AND,
    IT_ARR, // Illegal
    IT_ASL,
    IT_AXS, // Illegal
    IT_BIT,
    IT_BRK,
    IT_CLC,
    IT_CLD,
    IT_CLI,
    IT_CLV,
    IT_CMP,
    IT_CPX,
    IT_CPY,
    IT_DCP, // Illegal
    IT_DEC,
    IT_DEX,
    IT_DEY,
    IT_EOR,
    IT_IGN, // Illegal
    IT_INC,
    IT_INX,
    IT_INY,
    IT_ISC, // Illegal
    IT_JMP,
    IT_JSR,
    IT_LAX, // Illegal
    IT_LDA,
    IT_LDX,
    IT_LDY,
    IT_LSR,
    IT_NOP,
    IT_ORA,
    IT_EOL,
    IT_RLA, // Illegal
    IT_RRA, // Illegal
    IT_ROR,
    IT_RTI,
    IT_RTS,
    IT_SAX, // Illegal
    IT_SBC,
    IT_SEC,
    IT_SED,
    IT_SEI,
    IT_SKB, // Illegal
    IT_SLO, // Illegal
    IT_SRE, // Illegal
    IT_STA,
    IT_STX,
    IT_STY,
    IT_TAX,
    IT_TAY,
    IT_TXA,
    IT_TYA,
} InstructionType;

typedef uint8_t(instruction_load_t)(Cpu *, uint16_t);
typedef void(instruction_store_t)(Cpu *, uint16_t, uint8_t);

typedef struct
{
    instruction_load_t *load;
    instruction_store_t *store;
    size_t size;
} CpuLoadStore;

typedef void(instruction_func_t)(Cpu *, CpuLoadStore, uint16_t);

typedef struct Instruction
{
    const InstructionType type;
    const AddressMode address_mode;
    const uint64_t cycles;
} Instruction;

const char *cpu_inst_name(InstructionType type);
void cpu_print(Cpu *cpu);
void cpu_init(Cpu *cpu, Memory *mem);
void cpu_reset(Cpu *cpu);
void cpu_execute(Cpu *cpu);
void cpu_set_z(Cpu *cpu, uint8_t value);
void cpu_set_n(Cpu *cpu, uint8_t value);
uint16_t cpu_read16(Cpu *cpu, uint16_t address);
uint8_t u16_lo(uint16_t u16);
uint8_t u16_hi(uint16_t u16);
uint16_t u16_from_lohi(uint8_t lo, uint8_t hi);

instruction_load_t cpu_load_absolute;
instruction_load_t cpu_load_absolute_x;
instruction_load_t cpu_load_absolute_y;
instruction_load_t cpu_load_accumulator;
instruction_load_t cpu_load_immediate;
instruction_load_t cpu_load_implied;
instruction_load_t cpu_load_indirect;
instruction_load_t cpu_load_indirect_x;
instruction_load_t cpu_load_indirect_y;
instruction_load_t cpu_load_relative;
instruction_load_t cpu_load_zeropage;
instruction_load_t cpu_load_zeropage_x;
instruction_load_t cpu_load_zeropage_y;

instruction_store_t cpu_store_absolute;
instruction_store_t cpu_store_absolute_x;
instruction_store_t cpu_store_absolute_y;
instruction_store_t cpu_store_accumulator;
instruction_store_t cpu_store_immediate;
instruction_store_t cpu_store_implied;
instruction_store_t cpu_store_indirect;
instruction_store_t cpu_store_indirect_x;
instruction_store_t cpu_store_indirect_y;
instruction_store_t cpu_store_relative;
instruction_store_t cpu_store_zeropage;
instruction_store_t cpu_store_zeropage_x;
instruction_store_t cpu_store_zeropage_y;

instruction_func_t cpu_adc;
instruction_func_t cpu_and;
instruction_func_t cpu_asl;
instruction_func_t cpu_brk;
instruction_func_t cpu_inx;
instruction_func_t cpu_lda;
instruction_func_t cpu_sta;
instruction_func_t cpu_tax;

bool u8sign(uint8_t value);

#endif // INCLUDE_MOS6502_CPU_H_
