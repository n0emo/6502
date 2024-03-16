#ifndef INCLUDE_MOS6502_CPU_H_
#define INCLUDE_MOS6502_CPU_H_

#define CPU_RESET_VECTOR_1 0xFFFC
#define CPU_RESET_VECTOR_2 0xFFFD

#include "mem.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
    IT_ANE,
    IT_ARR, // Illegal
    IT_ASL,
    IT_AXS, // Illegal
    IT_BCC,
    IT_BCS,
    IT_BEQ,
    IT_BIT,
    IT_BMI,
    IT_BNE,
    IT_BPL,
    IT_BRK,
    IT_BVC,
    IT_BVS,
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
    IT_JAM, // Illegal
    IT_JMP,
    IT_JSR,
    IT_LAX, // Illegal
    IT_LDA,
    IT_LDX,
    IT_LDY,
    IT_LSR,
    IT_LXA, // Illegal
    IT_NOP,
    IT_ORA,
    IT_PHA,
    IT_PHP,
    IT_PLA,
    IT_PLP,
    IT_RLA, // Illegal
    IT_ROL,
    IT_ROR,
    IT_RRA, // Illegal
    IT_RTI,
    IT_RTS,
    IT_SAX, // Illegal
    IT_SBC,
    IT_SBX, // Illegal
    IT_SEC,
    IT_SED,
    IT_SEI,
    IT_SHA, // Illegal
    IT_SHX, // Illegal
    IT_SHY, // Illegal
    IT_SKB, // Illegal
    IT_SLO, // Illegal
    IT_SRE, // Illegal
    IT_STA,
    IT_STX,
    IT_STY,
    IT_TAS, // Illegal
    IT_TAX,
    IT_TAY,
    IT_TSX,
    IT_TXA,
    IT_TXS,
    IT_TYA,
    IT_USBC, // Illegal
} InstructionType;

typedef uint8_t(instruction_load_t)(Cpu *cpu, uint16_t operand);
typedef void(instruction_store_t)(Cpu *cpu, uint16_t operand, uint8_t value);
typedef uint16_t(instruction_address_t)(Cpu *cpu, uint16_t operand);

typedef struct
{
    instruction_load_t *const load;
    instruction_store_t *const store;
    instruction_address_t *const address;
    const size_t size;
} Addressing;

typedef void(instruction_func_t)(Cpu *, Addressing, uint16_t);

typedef struct Instruction
{
    const InstructionType type;
    const AddressMode address_mode;
    const uint64_t cycles;
    const bool increment_pc;
} Instruction;

const char *cpu_inst_name(InstructionType type);
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

instruction_address_t cpu_address_absolute;
instruction_address_t cpu_address_absolute_x;
instruction_address_t cpu_address_absolute_y;
instruction_address_t cpu_address_accumulator;
instruction_address_t cpu_address_immediate;
instruction_address_t cpu_address_implied;
instruction_address_t cpu_address_indirect;
instruction_address_t cpu_address_indirect_x;
instruction_address_t cpu_address_indirect_y;
instruction_address_t cpu_address_relative;
instruction_address_t cpu_address_zeropage;
instruction_address_t cpu_address_zeropage_x;
instruction_address_t cpu_address_zeropage_y;

instruction_func_t cpu_adc;
instruction_func_t cpu_and;
instruction_func_t cpu_asl;
instruction_func_t cpu_bcc;
instruction_func_t cpu_bcs;
instruction_func_t cpu_beq;
instruction_func_t cpu_bit;
instruction_func_t cpu_bmi;
instruction_func_t cpu_bne;
instruction_func_t cpu_bpl;
instruction_func_t cpu_brk;
instruction_func_t cpu_bvc;
instruction_func_t cpu_bvs;
instruction_func_t cpu_clc;
instruction_func_t cpu_cld;
instruction_func_t cpu_cli;
instruction_func_t cpu_clv;
instruction_func_t cpu_cmp;
instruction_func_t cpu_cmp;
instruction_func_t cpu_cpx;
instruction_func_t cpu_cpy;
instruction_func_t cpu_dec;
instruction_func_t cpu_dex;
instruction_func_t cpu_dey;
instruction_func_t cpu_eor;
instruction_func_t cpu_inc;
instruction_func_t cpu_inx;
instruction_func_t cpu_iny;
instruction_func_t cpu_jmp;
instruction_func_t cpu_jsr;
instruction_func_t cpu_lda;
instruction_func_t cpu_ldx;
instruction_func_t cpu_ldy;
instruction_func_t cpu_lsr;
instruction_func_t cpu_nop;
instruction_func_t cpu_ora;
instruction_func_t cpu_pha;
instruction_func_t cpu_php;
instruction_func_t cpu_pla;
instruction_func_t cpu_plp;
instruction_func_t cpu_rol;
instruction_func_t cpu_ror;
instruction_func_t cpu_rti;
instruction_func_t cpu_rts;
instruction_func_t cpu_sbc;
instruction_func_t cpu_sec;
instruction_func_t cpu_sed;
instruction_func_t cpu_sei;
instruction_func_t cpu_sta;
instruction_func_t cpu_stx;
instruction_func_t cpu_sty;
instruction_func_t cpu_tax;
instruction_func_t cpu_tay;
instruction_func_t cpu_tsx;
instruction_func_t cpu_txa;
instruction_func_t cpu_txs;
instruction_func_t cpu_tya;

#endif // INCLUDE_MOS6502_CPU_H_
