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

#endif // SRC_INSTRUCTIONS_H_
