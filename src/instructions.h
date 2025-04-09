#ifndef SRC_INSTRUCTIONS_H_
#define SRC_INSTRUCTIONS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "addressing.h"

typedef struct Cpu Cpu;

// Use with #define INSTRUCTION_X(uppercase, lowercase)
#define INSTRUCTION_LIST                  \
    INSTRUCTION_X(ADC, adc)               \
    INSTRUCTION_X(ALR, alr) /* Illegal */ \
    INSTRUCTION_X(ANC, anc) /* Illegal */ \
    INSTRUCTION_X(AND, and)               \
    INSTRUCTION_X(ANE, ane)               \
    INSTRUCTION_X(ARR, arr) /* Illegal */ \
    INSTRUCTION_X(ASL, asl)               \
    INSTRUCTION_X(AXS, axs) /* Illegal */ \
    INSTRUCTION_X(BCC, bcc)               \
    INSTRUCTION_X(BCS, bcs)               \
    INSTRUCTION_X(BEQ, beq)               \
    INSTRUCTION_X(BIT, bit)               \
    INSTRUCTION_X(BMI, bmi)               \
    INSTRUCTION_X(BNE, bne)               \
    INSTRUCTION_X(BPL, bpl)               \
    INSTRUCTION_X(BRK, brk)               \
    INSTRUCTION_X(BVC, bvc)               \
    INSTRUCTION_X(BVS, bvs)               \
    INSTRUCTION_X(CLC, clc)               \
    INSTRUCTION_X(CLD, cld)               \
    INSTRUCTION_X(CLI, cli)               \
    INSTRUCTION_X(CLV, clv)               \
    INSTRUCTION_X(CMP, cmp)               \
    INSTRUCTION_X(CPX, cpx)               \
    INSTRUCTION_X(CPY, cpy)               \
    INSTRUCTION_X(DCP, dcp) /* Illegal */ \
    INSTRUCTION_X(DEC, dec)               \
    INSTRUCTION_X(DEX, dex)               \
    INSTRUCTION_X(DEY, dey)               \
    INSTRUCTION_X(EOR, eor)               \
    INSTRUCTION_X(IGN, ign) /* Illegal */ \
    INSTRUCTION_X(INC, inc)               \
    INSTRUCTION_X(INX, inx)               \
    INSTRUCTION_X(INY, iny)               \
    INSTRUCTION_X(ISC, isc) /* Illegal */ \
    INSTRUCTION_X(JAM, jam) /* Illegal */ \
    INSTRUCTION_X(JMP, jmp)               \
    INSTRUCTION_X(JSR, jsr)               \
    INSTRUCTION_X(LAX, lax) /* Illegal */ \
    INSTRUCTION_X(LDA, lda)               \
    INSTRUCTION_X(LDX, ldx)               \
    INSTRUCTION_X(LDY, ldy)               \
    INSTRUCTION_X(LSR, lsr)               \
    INSTRUCTION_X(LXA, lxa) /* Illegal */ \
    INSTRUCTION_X(NOP, nop)               \
    INSTRUCTION_X(ORA, ora)               \
    INSTRUCTION_X(PHA, pha)               \
    INSTRUCTION_X(PHP, php)               \
    INSTRUCTION_X(PLA, pla)               \
    INSTRUCTION_X(PLP, plp)               \
    INSTRUCTION_X(RLA, rla) /* Illegal */ \
    INSTRUCTION_X(ROL, rol)               \
    INSTRUCTION_X(ROR, ror)               \
    INSTRUCTION_X(RRA, rra) /* Illegal */ \
    INSTRUCTION_X(RTI, rti)               \
    INSTRUCTION_X(RTS, rts)               \
    INSTRUCTION_X(SAX, sax) /* Illegal */ \
    INSTRUCTION_X(SBC, sbc)               \
    INSTRUCTION_X(SBX, sbx) /* Illegal */ \
    INSTRUCTION_X(SEC, sec)               \
    INSTRUCTION_X(SED, sed)               \
    INSTRUCTION_X(SEI, sei)               \
    INSTRUCTION_X(SHA, sha) /* Illegal */ \
    INSTRUCTION_X(SHX, shx) /* Illegal */ \
    INSTRUCTION_X(SHY, shy) /* Illegal */ \
    INSTRUCTION_X(SKB, skb) /* Illegal */ \
    INSTRUCTION_X(SLO, slo) /* Illegal */ \
    INSTRUCTION_X(SRE, sre) /* Illegal */ \
    INSTRUCTION_X(STA, sta)               \
    INSTRUCTION_X(STX, stx)               \
    INSTRUCTION_X(STY, sty)               \
    INSTRUCTION_X(TAS, tas) /* Illegal */ \
    INSTRUCTION_X(TAX, tax)               \
    INSTRUCTION_X(TAY, tay)               \
    INSTRUCTION_X(TSX, tsx)               \
    INSTRUCTION_X(TXA, txa)               \
    INSTRUCTION_X(TXS, txs)               \
    INSTRUCTION_X(TYA, tya)               \
    INSTRUCTION_X(USBC, usbc) /* Illegal */

typedef enum InstructionKind
{
#define INSTRUCTION_X(uppercase, ...) IT_##uppercase,
    INSTRUCTION_LIST
#undef INSTRUCTION_X
} InstructionKind;

typedef struct Instruction
{
    const InstructionKind type;
    const AddressMode address_mode;
    const uint64_t cycles;
    const bool increment_pc;
} Instruction;

typedef void(instruction_func_t)(Cpu *, Addressing, uint16_t);

#define INSTRUCTION_X(uppercase, lowercase, ...) instruction_func_t cpu_##lowercase;
INSTRUCTION_LIST
#undef INSTRUCTION_X

static inline const char *cpu_inst_name(InstructionKind kind)
{
    switch (kind)
    {
#define INSTRUCTION_X(uppercase, ...) \
    case IT_##uppercase:              \
        return #uppercase;
        INSTRUCTION_LIST
#undef INSTRUCTION_X
    default:
        abort();
    }
}

static inline instruction_func_t *get_instruction_func(InstructionKind kind)
{
    switch (kind)
    {
#define INSTRUCTION_X(uppercase, lowercase, ...) \
    case IT_##uppercase:                         \
        return &cpu_##lowercase;
        INSTRUCTION_LIST
#undef INSTRUCTION_X
    default:
        abort();
    }
}

// clang-format off
static inline Instruction get_instruction_by_opcode(uint8_t opcode)
{
    switch(opcode)
    {
        case 0x00: return (Instruction) {IT_BRK,  AM_IMPLIED,     7, true };
        case 0x01: return (Instruction) {IT_ORA,  AM_INDIRECT_X,  6, true };
        case 0x02: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x03: return (Instruction) {IT_SLO,  AM_INDIRECT_X,  8, true }; // Illegal
        case 0x04: return (Instruction) {IT_NOP,  AM_ZEROPAGE,    3, true }; // Illegal
        case 0x05: return (Instruction) {IT_ORA,  AM_ZEROPAGE,    3, true }; // manually tested
        case 0x06: return (Instruction) {IT_ASL,  AM_ZEROPAGE,    5, true }; // manually tested
        case 0x07: return (Instruction) {IT_SLO,  AM_ZEROPAGE,    5, true }; // Illegal
        case 0x08: return (Instruction) {IT_PHP,  AM_IMPLIED,     3, true };
        case 0x09: return (Instruction) {IT_ORA,  AM_IMMEDIATE,   2, true }; // manually tested
        case 0x0A: return (Instruction) {IT_ASL,  AM_ACCUMULATOR, 2, true }; // manually tested
        case 0x0B: return (Instruction) {IT_ANC,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0x0C: return (Instruction) {IT_NOP,  AM_ABSOLUTE,    4, true }; // Illegal
        case 0x0D: return (Instruction) {IT_ORA,  AM_ABSOLUTE,    4, true }; // manually tested
        case 0x0E: return (Instruction) {IT_ASL,  AM_ABSOLUTE,    6, true }; // manually tested
        case 0x0F: return (Instruction) {IT_SLO,  AM_ABSOLUTE,    6, true }; // Illegal

        case 0x10: return (Instruction) {IT_BPL,  AM_RELATIVE,    2, true };
        case 0x11: return (Instruction) {IT_ORA,  AM_INDIRECT_Y,  5, true };
        case 0x12: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x13: return (Instruction) {IT_SLO,  AM_INDIRECT_Y,  8, true }; // Illegal
        case 0x14: return (Instruction) {IT_NOP,  AM_ZEROPAGE,    4, true }; // Illegal
        case 0x15: return (Instruction) {IT_ORA,  AM_ZEROPAGE_X,  4, true };
        case 0x16: return (Instruction) {IT_ASL,  AM_ZEROPAGE_X,  6, true };
        case 0x17: return (Instruction) {IT_SLO,  AM_ZEROPAGE_X,  6, true }; // Illegal
        case 0x18: return (Instruction) {IT_CLC,  AM_IMPLIED,     2, true };
        case 0x19: return (Instruction) {IT_ORA,  AM_ABSOLUTE_Y,  4, true };
        case 0x1A: return (Instruction) {IT_NOP,  AM_IMPLIED,     2, true }; // Illegal
        case 0x1B: return (Instruction) {IT_SLO,  AM_ABSOLUTE_Y,  7, true };
        case 0x1C: return (Instruction) {IT_NOP,  AM_ABSOLUTE_X,  4, true }; // Illegal
        case 0x1D: return (Instruction) {IT_ORA,  AM_ABSOLUTE_X,  4, true };
        case 0x1E: return (Instruction) {IT_ASL,  AM_ABSOLUTE_X,  7, true };
        case 0x1F: return (Instruction) {IT_SLO,  AM_ABSOLUTE_X,  7, true }; // Illegal

        case 0x20: return (Instruction) {IT_JSR,  AM_ABSOLUTE,    6, true }; // manually tested
        case 0x21: return (Instruction) {IT_AND,  AM_INDIRECT_X,  6, true };
        case 0x22: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x23: return (Instruction) {IT_RLA,  AM_INDIRECT_X,  8, true }; // Illegal
        case 0x24: return (Instruction) {IT_BIT,  AM_ZEROPAGE,    3, true };
        case 0x25: return (Instruction) {IT_AND,  AM_ZEROPAGE,    3, true };
        case 0x26: return (Instruction) {IT_ROL,  AM_ZEROPAGE,    5, true };
        case 0x27: return (Instruction) {IT_RLA,  AM_ZEROPAGE,    5, true }; // Illegal
        case 0x28: return (Instruction) {IT_PLP,  AM_IMPLIED,     4, true };
        case 0x29: return (Instruction) {IT_AND,  AM_IMMEDIATE,   2, true };
        case 0x2A: return (Instruction) {IT_ROL,  AM_ACCUMULATOR, 2, true };
        case 0x2B: return (Instruction) {IT_ANC,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0x2C: return (Instruction) {IT_BIT,  AM_ABSOLUTE,    4, true };
        case 0x2D: return (Instruction) {IT_AND,  AM_ABSOLUTE,    4, true };
        case 0x2E: return (Instruction) {IT_ROL,  AM_ABSOLUTE,    6, true };
        case 0x2F: return (Instruction) {IT_RLA,  AM_ABSOLUTE,    6, true }; // Illegal

        case 0x30: return (Instruction) {IT_BMI,  AM_RELATIVE,    2, true };
        case 0x31: return (Instruction) {IT_AND,  AM_INDIRECT_Y,  5, true };
        case 0x32: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x33: return (Instruction) {IT_RLA,  AM_INDIRECT_Y,  8, true }; // Illegal
        case 0x34: return (Instruction) {IT_NOP,  AM_ZEROPAGE_X,  4, true }; // Illegal
        case 0x35: return (Instruction) {IT_AND,  AM_ZEROPAGE_X,  4, true };
        case 0x36: return (Instruction) {IT_ROL,  AM_ZEROPAGE_X,  6, true };
        case 0x37: return (Instruction) {IT_RLA,  AM_ZEROPAGE_X,  6, true }; // Illegal
        case 0x38: return (Instruction) {IT_SEC,  AM_IMPLIED,     2, true };
        case 0x39: return (Instruction) {IT_AND,  AM_ABSOLUTE_Y,  4, true };
        case 0x3A: return (Instruction) {IT_NOP,  AM_IMPLIED,     2, true }; // Illegal
        case 0x3B: return (Instruction) {IT_RLA,  AM_ABSOLUTE_Y,  7, true }; // Illegal
        case 0x3C: return (Instruction) {IT_NOP,  AM_ABSOLUTE_X,  4, true }; // Illegal
        case 0x3D: return (Instruction) {IT_AND,  AM_ABSOLUTE_X,  4, true };
        case 0x3E: return (Instruction) {IT_ROL,  AM_ABSOLUTE_X,  7, true };
        case 0x3F: return (Instruction) {IT_RLA,  AM_ABSOLUTE_X,  7, true }; // Illegal

        case 0x40: return (Instruction) {IT_RTI,  AM_IMPLIED,     6, false};
        case 0x41: return (Instruction) {IT_EOR,  AM_INDIRECT_X,  6, true };
        case 0x42: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x43: return (Instruction) {IT_SRE,  AM_INDIRECT_X,  8, true }; // Illegal
        case 0x44: return (Instruction) {IT_NOP,  AM_ZEROPAGE,    3, true }; // Illegal
        case 0x45: return (Instruction) {IT_EOR,  AM_ZEROPAGE,    3, true };
        case 0x46: return (Instruction) {IT_LSR,  AM_ZEROPAGE,    5, true };
        case 0x47: return (Instruction) {IT_SRE,  AM_ZEROPAGE,    5, true }; // Illegal
        case 0x48: return (Instruction) {IT_PHA,  AM_IMPLIED,     3, true }; // manually tested
        case 0x49: return (Instruction) {IT_EOR,  AM_IMMEDIATE,   2, true };
        case 0x4A: return (Instruction) {IT_LSR,  AM_ACCUMULATOR, 2, true };
        case 0x4B: return (Instruction) {IT_ALR,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0x4C: return (Instruction) {IT_JMP,  AM_ABSOLUTE,    3, false}; // manually tested
        case 0x4D: return (Instruction) {IT_EOR,  AM_ABSOLUTE,    4, true };
        case 0x4E: return (Instruction) {IT_LSR,  AM_ABSOLUTE,    6, true };
        case 0x4F: return (Instruction) {IT_SRE,  AM_ABSOLUTE,    6, true }; // Illegal

        case 0x50: return (Instruction) {IT_BVC,  AM_RELATIVE,    2, true };
        case 0x51: return (Instruction) {IT_EOR,  AM_INDIRECT_Y,  5, true };
        case 0x52: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x53: return (Instruction) {IT_SRE,  AM_INDIRECT_Y,  8, true }; // Illegal
        case 0x54: return (Instruction) {IT_NOP,  AM_ZEROPAGE_X,  4, true }; // Illegal
        case 0x55: return (Instruction) {IT_EOR,  AM_ZEROPAGE_X,  4, true };
        case 0x56: return (Instruction) {IT_LSR,  AM_ZEROPAGE_X,  6, true };
        case 0x57: return (Instruction) {IT_SRE,  AM_ZEROPAGE_X,  6, true }; // Illegal
        case 0x58: return (Instruction) {IT_CLI,  AM_IMPLIED,     2, true };
        case 0x59: return (Instruction) {IT_EOR,  AM_ABSOLUTE_Y,  4, true };
        case 0x5A: return (Instruction) {IT_NOP,  AM_IMPLIED,     2, true }; // Illegal
        case 0x5B: return (Instruction) {IT_SRE,  AM_ABSOLUTE_Y,  7, true }; // Illegal
        case 0x5C: return (Instruction) {IT_NOP,  AM_ABSOLUTE_X,  4, true }; // Illegal
        case 0x5D: return (Instruction) {IT_EOR,  AM_ABSOLUTE_X,  4, true };
        case 0x5E: return (Instruction) {IT_LSR,  AM_ABSOLUTE_X,  7, true };
        case 0x5F: return (Instruction) {IT_SRE,  AM_ABSOLUTE_X,  7, true }; // Illegal

        case 0x60: return (Instruction) {IT_RTS,  AM_IMPLIED,     6, false}; // manually tested
        case 0x61: return (Instruction) {IT_ADC,  AM_INDIRECT_X,  6, true };
        case 0x62: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x63: return (Instruction) {IT_RRA,  AM_INDIRECT_X,  8, true }; // Illegal
        case 0x64: return (Instruction) {IT_NOP,  AM_ZEROPAGE,    3, true }; // Illegal
        case 0x65: return (Instruction) {IT_ADC,  AM_ZEROPAGE,    3, true };
        case 0x66: return (Instruction) {IT_ROR,  AM_ZEROPAGE,    5, true };
        case 0x67: return (Instruction) {IT_RRA,  AM_ZEROPAGE,    5, true }; // Illegal
        case 0x68: return (Instruction) {IT_PLA,  AM_IMPLIED,     4, true }; // manually tested
        case 0x69: return (Instruction) {IT_ADC,  AM_IMMEDIATE,   2, true };
        case 0x6A: return (Instruction) {IT_ROR,  AM_ACCUMULATOR, 2, true };
        case 0x6B: return (Instruction) {IT_ARR,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0x6C: return (Instruction) {IT_JMP,  AM_INDIRECT,    5, false};
        case 0x6D: return (Instruction) {IT_ADC,  AM_ABSOLUTE,    4, true };
        case 0x6E: return (Instruction) {IT_ROR,  AM_ABSOLUTE,    6, true };
        case 0x6F: return (Instruction) {IT_RRA,  AM_ABSOLUTE,    6, true }; // Illegal

        case 0x70: return (Instruction) {IT_BVS,  AM_RELATIVE,    2, true };
        case 0x71: return (Instruction) {IT_ADC,  AM_INDIRECT_Y,  5, true };
        case 0x72: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x73: return (Instruction) {IT_RRA,  AM_INDIRECT_Y,  8, true }; // Illegal
        case 0x74: return (Instruction) {IT_NOP,  AM_ZEROPAGE_X,  4, true }; // Illegal
        case 0x75: return (Instruction) {IT_ADC,  AM_ZEROPAGE_X,  4, true };
        case 0x76: return (Instruction) {IT_ROR,  AM_ZEROPAGE_X,  6, true };
        case 0x77: return (Instruction) {IT_RRA,  AM_ZEROPAGE_X,  6, true }; // Illegal
        case 0x78: return (Instruction) {IT_SEI,  AM_IMPLIED,     2, true };
        case 0x79: return (Instruction) {IT_ADC,  AM_ABSOLUTE_Y,  4, true };
        case 0x7A: return (Instruction) {IT_NOP,  AM_IMPLIED,     2, true }; // Illegal
        case 0x7B: return (Instruction) {IT_RRA,  AM_ABSOLUTE_Y,  7, true }; // Illegal
        case 0x7C: return (Instruction) {IT_NOP,  AM_ABSOLUTE_X,  4, true }; // Illegal
        case 0x7D: return (Instruction) {IT_ADC,  AM_ABSOLUTE_X,  4, true };
        case 0x7E: return (Instruction) {IT_ROR,  AM_ABSOLUTE_X,  7, true };
        case 0x7F: return (Instruction) {IT_RRA,  AM_ABSOLUTE_X,  7, true }; // Illegal

        case 0x80: return (Instruction) {IT_NOP,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0x81: return (Instruction) {IT_STA,  AM_INDIRECT_X,  6, true };
        case 0x82: return (Instruction) {IT_NOP,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0x83: return (Instruction) {IT_SAX,  AM_INDIRECT_X,  6, true }; // Illegal
        case 0x84: return (Instruction) {IT_STY,  AM_ZEROPAGE,    3, true };
        case 0x85: return (Instruction) {IT_STA,  AM_ZEROPAGE,    3, true }; // manually tested
        case 0x86: return (Instruction) {IT_STX,  AM_ZEROPAGE,    3, true };
        case 0x87: return (Instruction) {IT_SAX,  AM_ZEROPAGE,    3, true }; // Illegal
        case 0x88: return (Instruction) {IT_DEY,  AM_IMPLIED,     2, true };
        case 0x89: return (Instruction) {IT_NOP,  AM_IMMEDIATE,   2, true };
        case 0x8A: return (Instruction) {IT_TXA,  AM_IMPLIED,     2, true }; // manually tested
        case 0x8B: return (Instruction) {IT_ANE,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0x8C: return (Instruction) {IT_STY,  AM_ABSOLUTE,    4, true }; // manually tested
        case 0x8D: return (Instruction) {IT_STA,  AM_ABSOLUTE,    4, true };
        case 0x8E: return (Instruction) {IT_STX,  AM_ABSOLUTE,    4, true }; // manually tested
        case 0x8F: return (Instruction) {IT_SAX,  AM_ABSOLUTE,    4, true }; // Illegal

        case 0x90: return (Instruction) {IT_BCC,  AM_RELATIVE,    2, true };
        case 0x91: return (Instruction) {IT_STA,  AM_INDIRECT_Y,  6, true };
        case 0x92: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0x93: return (Instruction) {IT_SHA,  AM_INDIRECT_Y,  6, true }; // Illegal
        case 0x94: return (Instruction) {IT_STY,  AM_ZEROPAGE_X,  4, true };
        case 0x95: return (Instruction) {IT_STA,  AM_ZEROPAGE_X,  4, true };
        case 0x96: return (Instruction) {IT_STX,  AM_ZEROPAGE_Y,  4, true };
        case 0x97: return (Instruction) {IT_SAX,  AM_ZEROPAGE_Y,  4, true }; // Illegal
        case 0x98: return (Instruction) {IT_TYA,  AM_IMPLIED,     2, true };
        case 0x99: return (Instruction) {IT_STA,  AM_ABSOLUTE_Y,  5, true };
        case 0x9A: return (Instruction) {IT_TXS,  AM_IMPLIED,     2, true }; // manually tested
        case 0x9B: return (Instruction) {IT_TAS,  AM_ABSOLUTE_Y,  5, true }; // Illegal
        case 0x9C: return (Instruction) {IT_SHY,  AM_ABSOLUTE_X,  5, true }; // Illegal
        case 0x9D: return (Instruction) {IT_STA,  AM_ABSOLUTE_X,  5, true };
        case 0x9E: return (Instruction) {IT_SHX,  AM_ABSOLUTE_Y,  5, true }; // Illegal
        case 0x9F: return (Instruction) {IT_SHA,  AM_ABSOLUTE_Y,  5, true }; // Illegal

        case 0xA0: return (Instruction) {IT_LDY,  AM_IMMEDIATE,   2, true }; // manually tested
        case 0xA1: return (Instruction) {IT_LDA,  AM_INDIRECT_X,  6, true }; // manually tested
        case 0xA2: return (Instruction) {IT_LDX,  AM_IMMEDIATE,   2, true }; // manually tested
        case 0xA3: return (Instruction) {IT_LAX,  AM_INDIRECT_X,  6, true }; // Illegal
        case 0xA4: return (Instruction) {IT_LDY,  AM_ZEROPAGE,    3, true };
        case 0xA5: return (Instruction) {IT_LDA,  AM_ZEROPAGE,    3, true };
        case 0xA6: return (Instruction) {IT_LDX,  AM_ZEROPAGE,    3, true };
        case 0xA7: return (Instruction) {IT_LAX,  AM_ZEROPAGE,    3, true }; // Illegal
        case 0xA8: return (Instruction) {IT_TAY,  AM_IMPLIED,     2, true };
        case 0xA9: return (Instruction) {IT_LDA,  AM_IMMEDIATE,   2, true }; // manually tested
        case 0xAA: return (Instruction) {IT_TAX,  AM_IMPLIED,     2, true };
        case 0xAB: return (Instruction) {IT_LXA,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0xAC: return (Instruction) {IT_LDY,  AM_ABSOLUTE,    4, true };
        case 0xAD: return (Instruction) {IT_LDA,  AM_ABSOLUTE,    4, true };
        case 0xAE: return (Instruction) {IT_LDX,  AM_ABSOLUTE,    4, true };
        case 0xAF: return (Instruction) {IT_LAX,  AM_ABSOLUTE,    4, true }; // Illegal

        case 0xB0: return (Instruction) {IT_BCS,  AM_RELATIVE,    2, true };
        case 0xB1: return (Instruction) {IT_LDA,  AM_INDIRECT_Y,  5, true }; // manually tested
        case 0xB2: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0xB3: return (Instruction) {IT_LAX,  AM_INDIRECT_Y,  5, true }; // Illegal
        case 0xB4: return (Instruction) {IT_LDY,  AM_ZEROPAGE_X,  4, true };
        case 0xB5: return (Instruction) {IT_LDA,  AM_ZEROPAGE_X,  4, true };
        case 0xB6: return (Instruction) {IT_LDX,  AM_ZEROPAGE_Y,  4, true };
        case 0xB7: return (Instruction) {IT_LAX,  AM_ZEROPAGE_Y,  4, true }; // Illegal
        case 0xB8: return (Instruction) {IT_CLV,  AM_IMPLIED,     2, true };
        case 0xB9: return (Instruction) {IT_LDA,  AM_ABSOLUTE_Y,  4, true };
        case 0xBA: return (Instruction) {IT_TSX,  AM_IMPLIED,     2, true };
        case 0xBB: return (Instruction) {IT_LAX,  AM_ABSOLUTE_Y,  4, true }; // Illegal
        case 0xBC: return (Instruction) {IT_LDY,  AM_ABSOLUTE_X,  4, true };
        case 0xBD: return (Instruction) {IT_LDA,  AM_ABSOLUTE_X,  4, true };
        case 0xBE: return (Instruction) {IT_LDX,  AM_ABSOLUTE_Y,  4, true };
        case 0xBF: return (Instruction) {IT_LAX,  AM_ABSOLUTE_Y,  4, true }; // Illegal

        case 0xC0: return (Instruction) {IT_CPY,  AM_IMMEDIATE,   2, true }; // manually tested
        case 0xC1: return (Instruction) {IT_CMP,  AM_INDIRECT_X,  6, true };
        case 0xC2: return (Instruction) {IT_NOP,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0xC3: return (Instruction) {IT_DCP,  AM_INDIRECT_X,  8, true }; // Illegal
        case 0xC4: return (Instruction) {IT_CPY,  AM_ZEROPAGE,    3, true };
        case 0xC5: return (Instruction) {IT_CMP,  AM_ZEROPAGE,    3, true };
        case 0xC6: return (Instruction) {IT_DEC,  AM_ZEROPAGE,    5, true };
        case 0xC7: return (Instruction) {IT_DCP,  AM_ZEROPAGE,    5, true }; // Illegal
        case 0xC8: return (Instruction) {IT_INY,  AM_IMPLIED,     2, true }; // manually tested
        case 0xC9: return (Instruction) {IT_CMP,  AM_IMMEDIATE,   2, true }; // manually tested
        case 0xCA: return (Instruction) {IT_DEX,  AM_IMPLIED,     2, true };
        case 0xCB: return (Instruction) {IT_SBX,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0xCC: return (Instruction) {IT_CPY,  AM_ABSOLUTE,    4, true };
        case 0xCD: return (Instruction) {IT_CMP,  AM_ABSOLUTE,    4, true };
        case 0xCE: return (Instruction) {IT_DEC,  AM_ABSOLUTE,    6, true };
        case 0xCF: return (Instruction) {IT_DCP,  AM_ABSOLUTE,    6, true }; // Illegal

        case 0xD0: return (Instruction) {IT_BNE,  AM_RELATIVE,    2, true }; // manually tested
        case 0xD1: return (Instruction) {IT_CMP,  AM_INDIRECT_Y,  5, true };
        case 0xD2: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0xD3: return (Instruction) {IT_DCP,  AM_INDIRECT_Y,  8, true }; // Illegal
        case 0xD4: return (Instruction) {IT_NOP,  AM_ZEROPAGE_X,  4, true }; // Illegal
        case 0xD5: return (Instruction) {IT_CMP,  AM_ZEROPAGE_X,  4, true };
        case 0xD6: return (Instruction) {IT_DEC,  AM_ZEROPAGE_X,  6, true };
        case 0xD7: return (Instruction) {IT_DCP,  AM_ZEROPAGE_X,  6, true }; // Illegal
        case 0xD8: return (Instruction) {IT_CLD,  AM_IMPLIED,     2, true };
        case 0xD9: return (Instruction) {IT_CMP,  AM_ABSOLUTE_Y,  4, true };
        case 0xDA: return (Instruction) {IT_NOP,  AM_IMPLIED,     2, true }; // Illegal
        case 0xDB: return (Instruction) {IT_DCP,  AM_ABSOLUTE_X,  7, true }; // Illegal
        case 0xDC: return (Instruction) {IT_NOP,  AM_ABSOLUTE_X,  4, true }; // Illegal
        case 0xDD: return (Instruction) {IT_CMP,  AM_ABSOLUTE_X,  4, true };
        case 0xDE: return (Instruction) {IT_DEC,  AM_ABSOLUTE_X,  7, true };
        case 0xDF: return (Instruction) {IT_DCP,  AM_ABSOLUTE_X,  7, true }; // Illegal

        case 0xE0: return (Instruction) {IT_CPX,  AM_IMMEDIATE,   2, true };
        case 0xE1: return (Instruction) {IT_SBC,  AM_INDIRECT_X,  6, true };
        case 0xE2: return (Instruction) {IT_NOP,  AM_IMMEDIATE,   2, true }; // Illegal
        case 0xE3: return (Instruction) {IT_ISC,  AM_INDIRECT_X,  8, true }; // Illegal
        case 0xE4: return (Instruction) {IT_CPX,  AM_ZEROPAGE,    3, true };
        case 0xE5: return (Instruction) {IT_SBC,  AM_ZEROPAGE,    3, true };
        case 0xE6: return (Instruction) {IT_INC,  AM_ZEROPAGE,    5, true };
        case 0xE7: return (Instruction) {IT_ISC,  AM_ZEROPAGE,    5, true }; // Illegal
        case 0xE8: return (Instruction) {IT_INX,  AM_IMPLIED,     2, true }; // manually tested
        case 0xE9: return (Instruction) {IT_SBC,  AM_IMMEDIATE,   2, true };
        case 0xEA: return (Instruction) {IT_NOP,  AM_IMPLIED,     2, true }; // manually tested
        case 0xEB: return (Instruction) {IT_USBC, AM_IMPLIED,     2, true }; // Illegal
        case 0xEC: return (Instruction) {IT_CPX,  AM_ABSOLUTE,    4, true };
        case 0xED: return (Instruction) {IT_SBC,  AM_ABSOLUTE,    4, true };
        case 0xEE: return (Instruction) {IT_INC,  AM_ABSOLUTE,    6, true };
        case 0xEF: return (Instruction) {IT_ISC,  AM_ABSOLUTE,    6, true }; // Illegal

        case 0xF0: return (Instruction) {IT_BEQ,  AM_RELATIVE,    2, true };
        case 0xF1: return (Instruction) {IT_SBC,  AM_INDIRECT_Y,  5, true };
        case 0xF2: return (Instruction) {IT_JAM,  AM_IMPLIED,     2, false}; // Illegal
        case 0xF3: return (Instruction) {IT_ISC,  AM_INDIRECT_Y,  8, true }; // Illegal
        case 0xF4: return (Instruction) {IT_NOP,  AM_ZEROPAGE_X,  4, true }; // Illegal
        case 0xF5: return (Instruction) {IT_SBC,  AM_ZEROPAGE_X,  4, true };
        case 0xF6: return (Instruction) {IT_INC,  AM_ZEROPAGE_X,  6, true };
        case 0xF7: return (Instruction) {IT_ISC,  AM_ZEROPAGE_X,  6, true }; // Illegal
        case 0xF8: return (Instruction) {IT_SED,  AM_IMPLIED,     2, true };
        case 0xF9: return (Instruction) {IT_SBC,  AM_ABSOLUTE_Y,  4, true };
        case 0xFA: return (Instruction) {IT_NOP,  AM_IMPLIED,     2, true }; // Illegal
        case 0xFB: return (Instruction) {IT_ISC,  AM_ABSOLUTE_Y,  7, true }; // Illegal
        case 0xFC: return (Instruction) {IT_NOP,  AM_ABSOLUTE_X,  4, true }; // Illegal
        case 0xFD: return (Instruction) {IT_SBC,  AM_ABSOLUTE_X,  4, true };
        case 0xFE: return (Instruction) {IT_INC,  AM_ABSOLUTE_X,  7, true };
        case 0xFF: return (Instruction) {IT_ISC,  AM_ABSOLUTE_X,  7, true }; // Illegal
    }
    abort();

    // clang-format on
}
#endif // SRC_INSTRUCTIONS_H_
