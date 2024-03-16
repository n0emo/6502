#include "cpu.h"
#include "int.h"
#include <stdio.h>

const Addressing addressings[] = {
    [AM_ABSOLUTE] = {
                     cpu_load_absolute,
                     cpu_store_absolute,
                     cpu_address_absolute,
                     3,
                     },
    [AM_ABSOLUTE_X] = {
                     cpu_load_absolute_x,
                     cpu_store_absolute_x,
                     cpu_address_absolute_x,
                     3,
                     },
    [AM_ABSOLUTE_Y] = {
                     cpu_load_absolute_y,
                     cpu_store_absolute_y,
                     cpu_address_absolute_y,
                     3,
                     },
    [AM_ACCUMULATOR] = {
                     cpu_load_accumulator,
                     cpu_store_accumulator,
                     cpu_address_accumulator,
                     1,
                     },
    [AM_IMMEDIATE] = {
                     cpu_load_immediate,
                     cpu_store_immediate,
                     cpu_address_immediate,
                     2,
                     },
    [AM_IMPLIED] = {
                     cpu_load_implied,
                     cpu_store_implied,
                     cpu_address_implied,
                     1,
                     },
    [AM_INDIRECT] = {
                     cpu_load_indirect,
                     cpu_store_indirect,
                     cpu_address_indirect,
                     3,
                     },
    [AM_INDIRECT_X] = {
                     cpu_load_indirect_x,
                     cpu_store_absolute_x,
                     cpu_address_indirect_x,
                     2,
                     },
    [AM_INDIRECT_Y] = {
                     cpu_load_indirect_y,
                     cpu_store_indirect_y,
                     cpu_address_indirect_y,
                     2,
                     },
    [AM_RELATIVE] = {
                     cpu_load_relative,
                     cpu_store_relative,
                     cpu_address_relative,
                     2,
                     },
    [AM_ZEROPAGE] = {
                     cpu_load_zeropage,
                     cpu_store_zeropage,
                     cpu_address_zeropage,
                     2,
                     },
    [AM_ZEROPAGE_X] = {
                     cpu_load_zeropage_x,
                     cpu_store_zeropage_x,
                     cpu_address_zeropage_x,
                     2,
                     },
    [AM_ZEROPAGE_Y] = {
                     cpu_load_zeropage_y,
                     cpu_store_zeropage_y,
                     cpu_address_zeropage_y,
                     2,
                     },
};

instruction_func_t *const inst_funcs[] = {
    [IT_ADC] = cpu_adc,
    [IT_ALR] = NULL, // Illegal
    [IT_ANC] = NULL, // Illegal
    [IT_AND] = cpu_and,
    [IT_ANE] = NULL, // Illegal
    [IT_ARR] = NULL, // Illegal
    [IT_ASL] = cpu_asl,
    [IT_AXS] = NULL, // Illegal
    [IT_BCC] = cpu_bcc,
    [IT_BCS] = cpu_bcs,
    [IT_BEQ] = cpu_beq,
    [IT_BIT] = cpu_bit,
    [IT_BMI] = cpu_bmi,
    [IT_BNE] = cpu_bne,
    [IT_BPL] = cpu_bpl,
    [IT_BRK] = cpu_brk,
    [IT_BVC] = cpu_bvc,
    [IT_BVS] = cpu_bvs,
    [IT_CLC] = cpu_clc,
    [IT_CLD] = cpu_cld,
    [IT_CLI] = cpu_cli,
    [IT_CLV] = cpu_clv,
    [IT_CMP] = cpu_cmp,
    [IT_CPX] = cpu_cpx,
    [IT_CPY] = cpu_cpy,
    [IT_DCP] = NULL, // Illegal
    [IT_DEC] = cpu_dec,
    [IT_DEX] = cpu_dex,
    [IT_DEY] = cpu_dey,
    [IT_EOR] = cpu_eor,
    [IT_IGN] = NULL, // Illegal
    [IT_INC] = cpu_inc,
    [IT_INX] = cpu_inx,
    [IT_INY] = cpu_iny,
    [IT_ISC] = NULL, // Illegal
    [IT_JAM] = NULL, // Illegal
    [IT_JMP] = cpu_jmp,
    [IT_JSR] = cpu_jsr,
    [IT_LAX] = NULL, // Illegal
    [IT_LDA] = cpu_lda,
    [IT_LDX] = cpu_ldx,
    [IT_LDY] = cpu_ldy,
    [IT_LSR] = cpu_lsr,
    [IT_LXA] = NULL, // Illegal
    [IT_NOP] = cpu_nop,
    [IT_ORA] = cpu_ora,
    [IT_PHA] = cpu_pha,
    [IT_PHP] = cpu_php,
    [IT_PLA] = cpu_pla,
    [IT_PLP] = cpu_plp,
    [IT_RLA] = NULL, // Illegal
    [IT_ROL] = cpu_rol,
    [IT_ROR] = cpu_ror,
    [IT_RRA] = NULL, // Illegal
    [IT_RTI] = cpu_rti,
    [IT_RTS] = cpu_rts,
    [IT_SAX] = NULL, // Illegal
    [IT_SBC] = cpu_sbc,
    [IT_SBX] = NULL, // Illegal
    [IT_SEC] = cpu_sec,
    [IT_SED] = cpu_sed,
    [IT_SEI] = cpu_sei,
    [IT_SHA] = NULL, // Illegal
    [IT_SHX] = NULL, // Illegal
    [IT_SHY] = NULL, // Illegal
    [IT_SKB] = NULL, // Illegal
    [IT_SLO] = NULL, // Illegal
    [IT_SRE] = NULL, // Illegal
    [IT_STA] = cpu_sta,
    [IT_STX] = cpu_stx,
    [IT_STY] = cpu_stx,
    [IT_TAS] = NULL, // Illegal
    [IT_TAX] = cpu_tax,
    [IT_TAY] = cpu_tay,
    [IT_TSX] = cpu_tsx,
    [IT_TXA] = cpu_txa,
    [IT_TXS] = cpu_txs,
    [IT_TYA] = cpu_tya,
    [IT_USBC] = NULL, // Illegal
};

const Instruction instructions[256] = {
    [0x00] = {IT_BRK,  AM_IMPLIED,     7, false},
    [0x01] = {IT_ORA,  AM_INDIRECT_X,  6, true },
    [0x02] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x03] = {IT_SLO,  AM_INDIRECT_X,  8, true }, // Illegal
    [0x04] = {IT_NOP,  AM_ZEROPAGE,    3, true }, // Illegal
    [0x05] = {IT_ORA,  AM_ZEROPAGE,    3, true },
    [0x06] = {IT_ASL,  AM_ZEROPAGE,    5, true },
    [0x07] = {IT_SLO,  AM_ZEROPAGE,    5, true }, // Illegal
    [0x08] = {IT_PHP,  AM_IMPLIED,     3, true },
    [0x09] = {IT_ORA,  AM_IMMEDIATE,   2, true },
    [0x0A] = {IT_ASL,  AM_ACCUMULATOR, 2, true },
    [0x0B] = {IT_ANC,  AM_IMMEDIATE,   2, true }, // Illegal
    [0x0C] = {IT_NOP,  AM_ABSOLUTE,    4, true }, // Illegal
    [0x0D] = {IT_ORA,  AM_ABSOLUTE,    4, true },
    [0x0E] = {IT_ASL,  AM_ABSOLUTE,    6, true },
    [0x0F] = {IT_SLO,  AM_ABSOLUTE,    6, true }, // Illegal

    [0x10] = {IT_BPL,  AM_RELATIVE,    2, false},
    [0x11] = {IT_ORA,  AM_INDIRECT_Y,  5, true },
    [0x12] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x13] = {IT_SLO,  AM_INDIRECT_Y,  8, true }, // Illegal
    [0x14] = {IT_NOP,  AM_ZEROPAGE,    4, true }, // Illegal
    [0x15] = {IT_ORA,  AM_ZEROPAGE_X,  4, true },
    [0x16] = {IT_ASL,  AM_ZEROPAGE_X,  6, true },
    [0x17] = {IT_SLO,  AM_ZEROPAGE_X,  6, true }, // Illegal
    [0x18] = {IT_CLC,  AM_IMPLIED,     2, true },
    [0x19] = {IT_ORA,  AM_ABSOLUTE_Y,  4, true },
    [0x1A] = {IT_NOP,  AM_IMPLIED,     2, true }, // Illegal
    [0x1B] = {IT_SLO,  AM_ABSOLUTE_Y,  7, true },
    [0x1C] = {IT_NOP,  AM_ABSOLUTE_X,  4, true }, // Illegal
    [0x1D] = {IT_ORA,  AM_ABSOLUTE_X,  4, true },
    [0x1E] = {IT_ASL,  AM_ABSOLUTE_X,  7, true },
    [0x1F] = {IT_SLO,  AM_ABSOLUTE_X,  7, true }, // Illegal

    [0x20] = {IT_JSR,  AM_ABSOLUTE,    6, false},
    [0x21] = {IT_AND,  AM_INDIRECT_X,  6, true },
    [0x22] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x23] = {IT_RLA,  AM_INDIRECT_X,  8, true }, // Illegal
    [0x24] = {IT_BIT,  AM_ZEROPAGE,    3, true },
    [0x25] = {IT_AND,  AM_ZEROPAGE,    3, true },
    [0x26] = {IT_ROL,  AM_ZEROPAGE,    5, true },
    [0x27] = {IT_RLA,  AM_ZEROPAGE,    5, true }, // Illegal
    [0x28] = {IT_PLP,  AM_IMPLIED,     4, true },
    [0x29] = {IT_AND,  AM_IMMEDIATE,   2, true },
    [0x2A] = {IT_ROL,  AM_ACCUMULATOR, 2, true },
    [0x2B] = {IT_ANC,  AM_IMMEDIATE,   2, true }, // Illegal
    [0x2C] = {IT_BIT,  AM_ABSOLUTE,    4, true },
    [0x2D] = {IT_AND,  AM_ABSOLUTE,    4, true },
    [0x2E] = {IT_ROL,  AM_ABSOLUTE,    6, true },
    [0x2F] = {IT_RLA,  AM_ABSOLUTE,    6, true }, // Illegal

    [0x30] = {IT_BMI,  AM_RELATIVE,    2, false},
    [0x31] = {IT_AND,  AM_INDIRECT_Y,  5, true },
    [0x32] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x33] = {IT_RLA,  AM_INDIRECT_Y,  8, true }, // Illegal
    [0x34] = {IT_NOP,  AM_ZEROPAGE_X,  4, true }, // Illegal
    [0x35] = {IT_AND,  AM_ZEROPAGE_X,  4, true },
    [0x36] = {IT_ROL,  AM_ZEROPAGE_X,  6, true },
    [0x37] = {IT_RLA,  AM_ZEROPAGE_X,  6, true }, // Illegal
    [0x38] = {IT_SEC,  AM_IMPLIED,     2, true },
    [0x39] = {IT_AND,  AM_ABSOLUTE_Y,  4, true },
    [0x3A] = {IT_NOP,  AM_IMPLIED,     2, true }, // Illegal
    [0x3B] = {IT_RLA,  AM_ABSOLUTE_Y,  7, true }, // Illegal
    [0x3C] = {IT_NOP,  AM_ABSOLUTE_X,  4, true }, // Illegal
    [0x3D] = {IT_AND,  AM_ABSOLUTE_X,  4, true },
    [0x3E] = {IT_ROL,  AM_ABSOLUTE_X,  7, true },
    [0x3F] = {IT_RLA,  AM_ABSOLUTE_X,  7, true }, // Illegal

    [0x40] = {IT_RTI,  AM_IMPLIED,     6, false},
    [0x41] = {IT_EOR,  AM_INDIRECT_X,  6, true },
    [0x42] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x43] = {IT_SRE,  AM_INDIRECT_X,  8, true }, // Illegal
    [0x44] = {IT_NOP,  AM_ZEROPAGE,    3, true }, // Illegal
    [0x45] = {IT_EOR,  AM_ZEROPAGE,    3, true },
    [0x46] = {IT_LSR,  AM_ZEROPAGE,    5, true },
    [0x47] = {IT_SRE,  AM_ZEROPAGE,    5, true }, // Illegal
    [0x48] = {IT_PHA,  AM_IMPLIED,     3, true },
    [0x49] = {IT_EOR,  AM_IMMEDIATE,   2, true },
    [0x4A] = {IT_LSR,  AM_ACCUMULATOR, 2, true },
    [0x4B] = {IT_ALR,  AM_IMMEDIATE,   2, true }, // Illegal
    [0x4C] = {IT_JMP,  AM_ABSOLUTE,    3, false},
    [0x4D] = {IT_EOR,  AM_ABSOLUTE,    4, true },
    [0x4E] = {IT_LSR,  AM_ABSOLUTE,    6, true },
    [0x4F] = {IT_SRE,  AM_ABSOLUTE,    6, true }, // Illegal

    [0x50] = {IT_BVC,  AM_RELATIVE,    2, false},
    [0x51] = {IT_EOR,  AM_INDIRECT_Y,  5, true },
    [0x52] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x53] = {IT_SRE,  AM_INDIRECT_Y,  8, true }, // Illegal
    [0x54] = {IT_NOP,  AM_ZEROPAGE_X,  4, true }, // Illegal
    [0x55] = {IT_EOR,  AM_ZEROPAGE_X,  4, true },
    [0x56] = {IT_LSR,  AM_ZEROPAGE_X,  6, true },
    [0x57] = {IT_SRE,  AM_ZEROPAGE_X,  6, true }, // Illegal
    [0x58] = {IT_CLI,  AM_IMPLIED,     2, true },
    [0x59] = {IT_EOR,  AM_ABSOLUTE_Y,  4, true },
    [0x5A] = {IT_NOP,  AM_IMPLIED,     2, true }, // Illegal
    [0x5B] = {IT_SRE,  AM_ABSOLUTE_Y,  7, true }, // Illegal
    [0x5C] = {IT_NOP,  AM_ABSOLUTE_X,  4, true }, // Illegal
    [0x5D] = {IT_EOR,  AM_ABSOLUTE_X,  4, true },
    [0x5E] = {IT_LSR,  AM_ABSOLUTE_X,  7, true },
    [0x5F] = {IT_SRE,  AM_ABSOLUTE_X,  7, true }, // Illegal

    [0x60] = {IT_RTS,  AM_IMPLIED,     6, false},
    [0x61] = {IT_ADC,  AM_INDIRECT_X,  6, true },
    [0x62] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x63] = {IT_RRA,  AM_INDIRECT_X,  8, true }, // Illegal
    [0x64] = {IT_NOP,  AM_ZEROPAGE,    3, true }, // Illegal
    [0x65] = {IT_ADC,  AM_ZEROPAGE,    3, true },
    [0x66] = {IT_ROR,  AM_ZEROPAGE,    5, true },
    [0x67] = {IT_RRA,  AM_ZEROPAGE,    5, true }, // Illegal
    [0x68] = {IT_PLA,  AM_IMPLIED,     4, true },
    [0x69] = {IT_ADC,  AM_IMMEDIATE,   2, true },
    [0x6A] = {IT_ROR,  AM_ACCUMULATOR, 2, true },
    [0x6B] = {IT_ARR,  AM_IMMEDIATE,   2, true }, // Illegal
    [0x6C] = {IT_JMP,  AM_INDIRECT,    5, false},
    [0x6D] = {IT_ADC,  AM_ABSOLUTE,    4, true },
    [0x6E] = {IT_ROR,  AM_ABSOLUTE,    6, true },
    [0x6F] = {IT_RRA,  AM_ABSOLUTE,    6, true }, // Illegal

    [0x70] = {IT_BVS,  AM_RELATIVE,    2, false},
    [0x71] = {IT_ADC,  AM_INDIRECT_Y,  5, true },
    [0x72] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x73] = {IT_RRA,  AM_INDIRECT_Y,  8, true }, // Illegal
    [0x74] = {IT_NOP,  AM_ZEROPAGE_X,  4, true }, // Illegal
    [0x75] = {IT_ADC,  AM_ZEROPAGE_X,  4, true },
    [0x76] = {IT_ROR,  AM_ZEROPAGE_X,  6, true },
    [0x77] = {IT_RRA,  AM_ZEROPAGE_X,  6, true }, // Illegal
    [0x78] = {IT_SEI,  AM_IMPLIED,     2, true },
    [0x79] = {IT_ADC,  AM_ABSOLUTE_Y,  4, true },
    [0x7A] = {IT_NOP,  AM_IMPLIED,     2, true }, // Illegal
    [0x7B] = {IT_RRA,  AM_ABSOLUTE_Y,  7, true }, // Illegal
    [0x7C] = {IT_NOP,  AM_ABSOLUTE_X,  4, true }, // Illegal
    [0x7D] = {IT_ADC,  AM_ABSOLUTE_X,  4, true },
    [0x7E] = {IT_ROR,  AM_ABSOLUTE_X,  7, true },
    [0x7F] = {IT_RRA,  AM_ABSOLUTE_X,  7, true }, // Illegal

    [0x80] = {IT_NOP,  AM_IMMEDIATE,   2, true }, // Illegal
    [0x81] = {IT_STA,  AM_INDIRECT_X,  6, true },
    [0x82] = {IT_NOP,  AM_IMMEDIATE,   2, true }, // Illegal
    [0x83] = {IT_SAX,  AM_INDIRECT_X,  6, true }, // Illegal
    [0x84] = {IT_STY,  AM_ZEROPAGE,    3, true },
    [0x85] = {IT_STA,  AM_ZEROPAGE,    3, true },
    [0x86] = {IT_STX,  AM_ZEROPAGE,    3, true },
    [0x87] = {IT_SAX,  AM_ZEROPAGE,    3, true }, // Illegal
    [0x88] = {IT_DEY,  AM_IMPLIED,     2, true },
    [0x89] = {IT_NOP,  AM_IMMEDIATE,   2, true },
    [0x8A] = {IT_TXA,  AM_IMPLIED,     2, true }, // Illegal
    [0x8B] = {IT_ANE,  AM_IMMEDIATE,   2, true }, // Illegal
    [0x8C] = {IT_STY,  AM_ABSOLUTE,    4, true }, // Illegal
    [0x8D] = {IT_STA,  AM_ABSOLUTE,    4, true },
    [0x8E] = {IT_STX,  AM_ABSOLUTE,    4, true },
    [0x8F] = {IT_SAX,  AM_ABSOLUTE,    4, true }, // Illegal

    [0x90] = {IT_BCC,  AM_RELATIVE,    2, false},
    [0x91] = {IT_STA,  AM_INDIRECT_Y,  6, true },
    [0x92] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0x93] = {IT_SHA,  AM_INDIRECT_Y,  6, true }, // Illegal
    [0x94] = {IT_STY,  AM_ZEROPAGE_X,  4, true },
    [0x95] = {IT_STA,  AM_ZEROPAGE_X,  4, true },
    [0x96] = {IT_STX,  AM_ZEROPAGE_Y,  4, true },
    [0x97] = {IT_SAX,  AM_ZEROPAGE_Y,  4, true }, // Illegal
    [0x98] = {IT_TYA,  AM_IMPLIED,     2, true },
    [0x99] = {IT_STA,  AM_ABSOLUTE_Y,  5, true },
    [0x9A] = {IT_TXS,  AM_IMPLIED,     2, true },
    [0x9B] = {IT_TAS,  AM_ABSOLUTE_Y,  5, true }, // Illegal
    [0x9C] = {IT_SHY,  AM_ABSOLUTE_X,  5, true }, // Illegal
    [0x9D] = {IT_STA,  AM_ABSOLUTE_X,  5, true },
    [0x9E] = {IT_SHX,  AM_ABSOLUTE_Y,  5, true }, // Illegal
    [0x9F] = {IT_SHA,  AM_ABSOLUTE_Y,  5, true }, // Illegal

    [0xA0] = {IT_LDY,  AM_IMMEDIATE,   2, true },
    [0xA1] = {IT_LDA,  AM_INDIRECT_X,  6, true },
    [0xA2] = {IT_LDX,  AM_IMMEDIATE,   2, true },
    [0xA3] = {IT_LAX,  AM_INDIRECT_X,  6, true }, // Illegal
    [0xA4] = {IT_LDY,  AM_ZEROPAGE,    3, true },
    [0xA5] = {IT_LDA,  AM_ZEROPAGE,    3, true },
    [0xA6] = {IT_LDX,  AM_ZEROPAGE,    3, true },
    [0xA7] = {IT_LAX,  AM_ZEROPAGE,    3, true }, // Illegal
    [0xA8] = {IT_TAY,  AM_IMPLIED,     2, true },
    [0xA9] = {IT_LDA,  AM_IMMEDIATE,   2, true },
    [0xAA] = {IT_TAX,  AM_IMPLIED,     2, true },
    [0xAB] = {IT_LXA,  AM_IMMEDIATE,   2, true }, // Illegal
    [0xAC] = {IT_LDY,  AM_ABSOLUTE,    4, true },
    [0xAD] = {IT_LDA,  AM_ABSOLUTE,    4, true },
    [0xAE] = {IT_LDX,  AM_ABSOLUTE,    4, true },
    [0xAF] = {IT_LAX,  AM_ABSOLUTE,    4, true }, // Illegal

    [0xB0] = {IT_BCS,  AM_RELATIVE,    2, false},
    [0xB1] = {IT_LDA,  AM_INDIRECT_Y,  5, true },
    [0xB2] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0xB3] = {IT_LAX,  AM_INDIRECT_Y,  5, true }, // Illegal
    [0xB4] = {IT_LDY,  AM_INDIRECT_X,  4, true },
    [0xB5] = {IT_LDA,  AM_ZEROPAGE_X,  4, true },
    [0xB6] = {IT_LDX,  AM_ZEROPAGE_Y,  4, true },
    [0xB7] = {IT_LAX,  AM_ZEROPAGE_Y,  4, true }, // Illegal
    [0xB8] = {IT_CLV,  AM_IMPLIED,     2, true },
    [0xB9] = {IT_LDA,  AM_ABSOLUTE_Y,  4, true },
    [0xBA] = {IT_TSX,  AM_IMPLIED,     2, true },
    [0xBB] = {IT_LAX,  AM_ABSOLUTE_Y,  4, true }, // Illegal
    [0xBC] = {IT_LDY,  AM_ABSOLUTE_X,  4, true },
    [0xBD] = {IT_LDA,  AM_ABSOLUTE_X,  4, true },
    [0xBE] = {IT_LDX,  AM_ABSOLUTE_Y,  4, true },
    [0xBF] = {IT_LAX,  AM_ABSOLUTE_Y,  4, true }, // Illegal

    [0xC0] = {IT_CPY,  AM_IMMEDIATE,   2, true },
    [0xC1] = {IT_CMP,  AM_INDIRECT_X,  6, true },
    [0xC2] = {IT_NOP,  AM_IMMEDIATE,   2, true }, // Illegal
    [0xC3] = {IT_DCP,  AM_INDIRECT_X,  8, true }, // Illegal
    [0xC4] = {IT_CPY,  AM_ZEROPAGE,    3, true },
    [0xC5] = {IT_CMP,  AM_ZEROPAGE,    3, true },
    [0xC6] = {IT_DEC,  AM_ZEROPAGE,    5, true },
    [0xC7] = {IT_DCP,  AM_ZEROPAGE,    5, true }, // Illegal
    [0xC8] = {IT_INY,  AM_IMPLIED,     2, true },
    [0xC9] = {IT_CMP,  AM_IMMEDIATE,   2, true },
    [0xCA] = {IT_DEX,  AM_IMPLIED,     2, true },
    [0xCB] = {IT_SBX,  AM_IMMEDIATE,   2, true }, // Illegal
    [0xCC] = {IT_CPY,  AM_ABSOLUTE,    4, true },
    [0xCD] = {IT_CMP,  AM_ABSOLUTE,    4, true },
    [0xCE] = {IT_DEC,  AM_ABSOLUTE,    6, true },
    [0xCF] = {IT_DCP,  AM_ABSOLUTE,    6, true }, // Illegal

    [0xD0] = {IT_BNE,  AM_RELATIVE,    2, false},
    [0xD1] = {IT_CMP,  AM_INDIRECT_Y,  5, true },
    [0xD2] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0xD3] = {IT_DCP,  AM_INDIRECT_Y,  8, true }, // Illegal
    [0xD4] = {IT_NOP,  AM_ZEROPAGE_X,  4, true }, // Illegal
    [0xD5] = {IT_CMP,  AM_ZEROPAGE_X,  4, true },
    [0xD6] = {IT_DEC,  AM_ZEROPAGE_X,  6, true },
    [0xD7] = {IT_DCP,  AM_ZEROPAGE_X,  6, true }, // Illegal
    [0xD8] = {IT_CLD,  AM_IMPLIED,     2, true },
    [0xD9] = {IT_CMP,  AM_ABSOLUTE_Y,  4, true },
    [0xDA] = {IT_NOP,  AM_IMPLIED,     2, true }, // Illegal
    [0xDB] = {IT_DCP,  AM_ABSOLUTE_X,  7, true }, // Illegal
    [0xDC] = {IT_NOP,  AM_ABSOLUTE_X,  4, true }, // Illegal
    [0xDD] = {IT_CMP,  AM_ABSOLUTE_X,  4, true },
    [0xDE] = {IT_DEC,  AM_ABSOLUTE_X,  7, true },
    [0xDF] = {IT_DCP,  AM_ABSOLUTE_X,  7, true }, // Illegal

    [0xE0] = {IT_CPX,  AM_IMMEDIATE,   2, true },
    [0xE1] = {IT_SBC,  AM_INDIRECT_X,  6, true },
    [0xE2] = {IT_NOP,  AM_IMMEDIATE,   2, true }, // Illegal
    [0xE3] = {IT_ISC,  AM_INDIRECT_X,  8, true }, // Illegal
    [0xE4] = {IT_CPX,  AM_ZEROPAGE,    3, true },
    [0xE5] = {IT_SBC,  AM_ZEROPAGE,    3, true },
    [0xE6] = {IT_INC,  AM_ZEROPAGE,    5, true },
    [0xE7] = {IT_ISC,  AM_ZEROPAGE,    5, true }, // Illegal
    [0xE8] = {IT_INX,  AM_IMPLIED,     2, true },
    [0xE9] = {IT_SBC,  AM_IMMEDIATE,   2, true },
    [0xEA] = {IT_NOP,  AM_IMPLIED,     2, true },
    [0xEB] = {IT_USBC, AM_IMPLIED,     2, true }, // Illegal
    [0xEC] = {IT_CPX,  AM_ABSOLUTE,    4, true },
    [0xED] = {IT_SBC,  AM_ABSOLUTE,    4, true },
    [0xEE] = {IT_INC,  AM_ABSOLUTE,    6, true },
    [0xEF] = {IT_ISC,  AM_ABSOLUTE,    6, true }, // Illegal

    [0xF0] = {IT_BEQ,  AM_RELATIVE,    2, false},
    [0xF1] = {IT_SBC,  AM_INDIRECT_Y,  5, true },
    [0xF2] = {IT_JAM,  AM_IMPLIED,     2, false}, // Illegal
    [0xF3] = {IT_ISC,  AM_INDIRECT_Y,  8, true }, // Illegal
    [0xF4] = {IT_NOP,  AM_ZEROPAGE_X,  4, true }, // Illegal
    [0xF5] = {IT_SBC,  AM_ZEROPAGE_X,  4, true },
    [0xF6] = {IT_INC,  AM_ZEROPAGE_X,  6, true },
    [0xF7] = {IT_ISC,  AM_ZEROPAGE_X,  6, true }, // Illegal
    [0xF8] = {IT_SED,  AM_IMPLIED,     2, true },
    [0xF9] = {IT_SBC,  AM_ABSOLUTE_Y,  4, true },
    [0xFA] = {IT_NOP,  AM_IMPLIED,     2, true }, // Illegal
    [0xFB] = {IT_ISC,  AM_ABSOLUTE_Y,  7, true }, // Illegal
    [0xFC] = {IT_NOP,  AM_ABSOLUTE_X,  4, true }, // Illegal
    [0xFD] = {IT_SBC,  AM_ABSOLUTE_X,  4, true },
    [0xFE] = {IT_INC,  AM_ABSOLUTE_X,  7, true },
    [0xFF] = {IT_ISC,  AM_ABSOLUTE_X,  7, true }, // Illegal
};

const char *const cpu_inst_names[] = {
    "ADC",
    "ALR", // Illegal
    "ANC", // Illegal
    "AND",
    "ANE",
    "ARR", // Illegal
    "ASL",
    "AXS", // Illegal
    "BCC",
    "BCS",
    "BEQ",
    "BIT",
    "BMI",
    "BNE",
    "BPL",
    "BRK",
    "BVC",
    "BVS",
    "CLC",
    "CLD",
    "CLI",
    "CLV",
    "CMP",
    "CPX",
    "CPY",
    "DCP", // Illegal
    "DEC",
    "DEX",
    "DEY",
    "EOR",
    "IGN", // Illegal
    "INC",
    "INX",
    "INY",
    "ISC", // Illegal
    "JAM", // Illegal
    "JMP",
    "JSR",
    "LAX", // Illegal
    "LDA",
    "LDX",
    "LDY",
    "LSR",
    "LXA", // Illegal
    "NOP",
    "ORA",
    "PHA",
    "PHP",
    "PLA",
    "PLP",
    "RLA", // Illegal
    "ROL",
    "ROR",
    "RRA", // Illegal
    "RTI",
    "RTS",
    "SAX", // Illegal
    "SBC",
    "SBX", // Illegal
    "SEC",
    "SED",
    "SEI",
    "SHA", // Illegal
    "SHX", // Illegal
    "SHY", // Illegal
    "SKB", // Illegal
    "SLO", // Illegal
    "SRE", // Illegal
    "STA",
    "STX",
    "STY",
    "TAS", // Illegal
    "TAX",
    "TAY",
    "TSX",
    "TXA",
    "TXS",
    "TYA",
    "USBC", // Illegal
};

void cpu_print(Cpu *cpu)
{
    printf(
        "PC=0x%.4x, SP=0x%.2x\n"
        "AC=0x%.2x, X=0x%.2x, Y=0x%.2x\n"
        "NV-BDIZC\n"
        "%d%d%d%d%d%d%d%d\n\n",
        cpu->PC, cpu->SP,
        cpu->A, cpu->X, cpu->Y,
        cpu->N, cpu->V, cpu->U, cpu->B, cpu->D, cpu->I, cpu->Z, cpu->C);
}

void cpu_init(Cpu *cpu, Memory *mem)
{
    cpu->mem = mem;

    cpu->PC = 0;
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->SP = 0;

    cpu->N = 0;
    cpu->V = 0;
    cpu->B = 0;
    cpu->D = 0;
    cpu->I = 0;
    cpu->Z = 0;
    cpu->C = 0;
}

void cpu_reset(Cpu *cpu)
{
    uint8_t pc_fst = mem_read(cpu->mem, CPU_RESET_VECTOR_1);
    uint8_t pc_snd = mem_read(cpu->mem, CPU_RESET_VECTOR_2);
    cpu->PC = (pc_fst << 8) | pc_snd;
}

void cpu_execute(Cpu *cpu)
{
    uint8_t inst_code = mem_read(cpu->mem, cpu->PC);
    Instruction instruction = instructions[inst_code];
    printf("Executing %s\n", cpu_inst_names[instruction.type]);
    size_t inst_size = addressings[instruction.address_mode].size;
    uint16_t data = mem_read16(cpu->mem, cpu->PC + 1);
    inst_funcs[instruction.type](cpu, addressings[instruction.address_mode], data);
    cpu->PC += inst_size;
    cpu_print(cpu);
}

uint8_t cpu_load_absolute(Cpu *cpu, uint16_t operand)
{
    uint16_t address = cpu_address_absolute(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_absolute_x(Cpu *cpu, uint16_t operand)
{
    uint16_t address = cpu_address_absolute_x(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_absolute_y(Cpu *cpu, uint16_t operand)
{
    uint16_t address = cpu_address_absolute_y(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_accumulator(Cpu *cpu, uint16_t operand)
{
    UNUSED(operand);
    return cpu->A;
}

uint8_t cpu_load_immediate(Cpu *cpu, uint16_t operand)
{
    UNUSED(cpu);
    return u16_lo(operand);
}

uint8_t cpu_load_implied(Cpu *cpu, uint16_t operand)
{
    UNUSED2(cpu, operand);
    return 0;
}

uint8_t cpu_load_indirect(Cpu *cpu, uint16_t operand)
{
    uint16_t address = cpu_address_indirect(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_indirect_x(Cpu *cpu, uint16_t operand)
{
    uint16_t address = cpu_address_indirect_x(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_indirect_y(Cpu *cpu, uint16_t operand)
{
    uint16_t address = cpu_address_indirect_y(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_relative(Cpu *cpu, uint16_t operand)
{
    uint16_t address = cpu_address_relative(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_zeropage(Cpu *cpu, uint16_t operand)
{
    uint8_t address = cpu_address_zeropage(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_zeropage_x(Cpu *cpu, uint16_t operand)
{
    uint8_t address = cpu_address_zeropage_x(cpu, operand);
    return mem_read(cpu->mem, address);
}

uint8_t cpu_load_zeropage_y(Cpu *cpu, uint16_t operand)
{
    uint8_t address = cpu_address_zeropage_y(cpu, operand);
    return mem_read(cpu->mem, address);
}

void cpu_store_absolute(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint16_t address = cpu_address_absolute(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_absolute_x(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint16_t address = cpu_address_absolute_x(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_absolute_y(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint16_t address = cpu_address_absolute_y(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_accumulator(Cpu *cpu, uint16_t operand, uint8_t value)
{
    UNUSED3(cpu, operand, value);
}

void cpu_store_immediate(Cpu *cpu, uint16_t operand, uint8_t value)
{
    UNUSED3(cpu, operand, value);
}

void cpu_store_implied(Cpu *cpu, uint16_t operand, uint8_t value)
{
    UNUSED3(cpu, operand, value);
}

void cpu_store_indirect(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint16_t address = cpu_address_indirect(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_indirect_x(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint16_t address = cpu_address_indirect_x(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_indirect_y(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint16_t address = cpu_address_indirect_y(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_relative(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint16_t address = cpu_address_relative(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_zeropage(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint8_t address = cpu_address_zeropage(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_zeropage_x(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint8_t address = cpu_address_zeropage_x(cpu, operand);
    mem_write(cpu->mem, address, value);
}

void cpu_store_zeropage_y(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint8_t address = cpu_address_zeropage_y(cpu, operand);
    mem_write(cpu->mem, address, value);
}

uint16_t cpu_address_absolute(Cpu *cpu, uint16_t operand)
{
    UNUSED(cpu);
    return operand;
}

uint16_t cpu_address_absolute_x(Cpu *cpu, uint16_t operand)
{
    return operand + cpu->X;
}

uint16_t cpu_address_absolute_y(Cpu *cpu, uint16_t operand)
{
    return operand + cpu->Y;
}

uint16_t cpu_address_accumulator(Cpu *cpu, uint16_t operand)
{
    UNUSED2(cpu, operand);
    return 0;
}

uint16_t cpu_address_immediate(Cpu *cpu, uint16_t operand)
{
    UNUSED2(cpu, operand);
    return 0;
}

uint16_t cpu_address_implied(Cpu *cpu, uint16_t operand)
{
    UNUSED2(cpu, operand);
    return 0;
}

uint16_t cpu_address_indirect(Cpu *cpu, uint16_t operand)
{
    return mem_read16(cpu->mem, operand);
}

uint16_t cpu_address_indirect_x(Cpu *cpu, uint16_t operand)
{
    uint8_t address_to_address = operand + cpu->X;
    return u16_lo(mem_read16(cpu->mem, address_to_address));
}

uint16_t cpu_address_indirect_y(Cpu *cpu, uint16_t operand)
{
    return u16_lo(mem_read16(cpu->mem, operand)) + cpu->Y;
}

uint16_t cpu_address_relative(Cpu *cpu, uint16_t operand)
{
    uint8_t unsigned_offset = u16_lo(operand);
    int8_t offset = *(int8_t *)&unsigned_offset;
    return cpu->PC + offset;
}

uint16_t cpu_address_zeropage(Cpu *cpu, uint16_t operand)
{
    UNUSED(cpu);
    return u16_lo(operand);
}

uint16_t cpu_address_zeropage_x(Cpu *cpu, uint16_t operand)
{
    return u16_lo(operand) + cpu->X;
}

uint16_t cpu_address_zeropage_y(Cpu *cpu, uint16_t operand)
{
    return u16_lo(operand) + cpu->Y;
}

void cpu_set_z(Cpu *cpu, uint8_t value)
{
    cpu->Z = value == 0;
}

void cpu_set_n(Cpu *cpu, uint8_t value)
{
    cpu->N = u8sign(value);
}

void cpu_set_zn(Cpu *cpu, uint8_t value)
{
    cpu_set_z(cpu, value);
    cpu_set_n(cpu, value);
}

uint8_t cpu_get_status(Cpu *cpu)
{
    return cpu->N << 7 |
           cpu->V << 6 |
           cpu->U << 5 |
           cpu->B << 4 |
           cpu->D << 3 |
           cpu->I << 2 |
           cpu->Z << 1 |
           cpu->C << 0;
}

void cpu_push(Cpu *cpu, uint8_t value)
{
    mem_write(cpu->mem, 0x100 | cpu->SP, value);
    cpu->SP--;
}

void cpu_push16(Cpu *cpu, uint16_t value)
{
    cpu_push(cpu, u16_hi(value));
    cpu_push(cpu, u16_lo(value));
}

uint8_t cpu_pull(Cpu *cpu)
{
    cpu->SP++;
    return mem_read(cpu->mem, 0x100 | cpu->SP);
}

uint16_t cpu_pull16(Cpu *cpu)
{
    uint8_t lo = cpu_pull(cpu);
    uint8_t hi = cpu_pull(cpu);
    return u16_from_lohi(lo, hi);
}

void cpu_set_status(Cpu *cpu, uint8_t status)
{
    cpu->N = status & (1 << 7);
    cpu->V = status & (1 << 6);
    cpu->U = status & (1 << 5);
    cpu->B = status & (1 << 4);
    cpu->D = status & (1 << 3);
    cpu->I = status & (1 << 2);
    cpu->Z = status & (1 << 1);
    cpu->C = status & (1 << 0);
}

void cpu_adc(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = cpu->A;
    uint8_t b = addressing.load(cpu, operand);
    bool c = cpu->C;
    cpu->A = a + b + c;
    cpu_set_zn(cpu, cpu->A);
    cpu->C = a + b + c > 0xFF;
    cpu->V = (u8sign(a) == u8sign(b) && u8sign(cpu->A) != u8sign(b));
}

void cpu_and(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t b = addressing.load(cpu, operand);
    cpu->A = cpu->A & b;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_asl(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    size_t a = addressing.load(cpu, operand);
    a = a << 1;
}

void cpu_bcc(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    if (cpu->C == 0)
    {
        cpu->PC = addressing.address(cpu, operand);
    }
}

void cpu_bcs(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    if (cpu->C == 1)
    {
        cpu->PC = addressing.address(cpu, operand);
    }
}

void cpu_beq(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    if (cpu->Z == 1)
    {
        cpu->PC = addressing.address(cpu, operand);
    }
}

void cpu_bit(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t result = cpu->A & addressing.load(cpu, operand);
    cpu_set_zn(cpu, result);
    cpu->V = result & (1 << 6);
}

void cpu_bmi(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    if (cpu->N == 1)
    {
        cpu->PC = addressing.address(cpu, operand);
    }
}

void cpu_bne(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    if (cpu->Z == 0)
    {
        cpu->PC = addressing.address(cpu, operand);
    }
}

void cpu_bpl(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    if (cpu->N == 0)
    {
        cpu->PC = addressing.address(cpu, operand);
    }
}

void cpu_brk(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->B = 0;
}

void cpu_bvc(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    if (cpu->V == 0)
    {
        cpu->PC = addressing.address(cpu, operand);
    }
}

void cpu_bvs(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    if (cpu->V == 1)
    {
        cpu->PC = addressing.address(cpu, operand);
    }
}

void cpu_clc(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->C = 0;
}

void cpu_cld(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->D = 0;
}

void cpu_cli(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->I = 0;
}

void cpu_clv(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->V = 0;
}

void cpu_cmp(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = cpu->A, b = addressing.load(cpu, operand);
    cpu_set_zn(cpu, a - b);
    cpu->C = a > b;
}

void cpu_cpx(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = cpu->X, b = addressing.load(cpu, operand);
    cpu_set_zn(cpu, a - b);
    cpu->C = a > b;
}

void cpu_cpy(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = cpu->Y, b = addressing.load(cpu, operand);
    cpu_set_zn(cpu, a - b);
    cpu->C = a > b;
}

void cpu_dec(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = addressing.load(cpu, operand);
    a--;
    addressing.store(cpu, operand, a);
}

void cpu_dex(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->X--;
}

void cpu_dey(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->Y--;
}

void cpu_eor(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    cpu->A = cpu->A ^ addressing.load(cpu, operand);
    cpu_set_zn(cpu, cpu->A);
}

void cpu_inc(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->A++;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_inx(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->X++;
    cpu_set_zn(cpu, cpu->X);
}

void cpu_iny(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->Y++;
    cpu_set_zn(cpu, cpu->Y);
}

void cpu_jmp(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    cpu->PC = addressing.address(cpu, operand);
}

void cpu_jsr(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    cpu_push16(cpu, cpu->PC);
    cpu->PC = addressing.address(cpu, operand);
}

void cpu_lda(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    cpu->A = addressing.load(cpu, operand);
    cpu_set_zn(cpu, cpu->A);
}

void cpu_ldx(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    cpu->X = addressing.load(cpu, operand);
    cpu_set_zn(cpu, cpu->X);
}

void cpu_ldy(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    cpu->Y = addressing.load(cpu, operand);
    cpu_set_zn(cpu, cpu->Y);
}

void cpu_lsr(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = addressing.load(cpu, operand);
    cpu->C = a & 1;
    a = a >> 1;
    cpu_set_zn(cpu, a);
    addressing.store(cpu, operand, a);
}

void cpu_nop(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED3(cpu, addressing, operand);
}

void cpu_ora(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    cpu->A = cpu->A | addressing.load(cpu, operand);
    cpu_set_zn(cpu, cpu->A);
}

void cpu_pha(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu_push(cpu, cpu->A);
}

void cpu_php(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu_push(cpu, cpu_get_status(cpu));
}

void cpu_pla(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->A = cpu_pull(cpu);
}

void cpu_plp(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu_set_status(cpu, cpu_pull(cpu));
}

void cpu_rol(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = addressing.load(cpu, operand);
    cpu->C = a & (1 << 7);
    a = (a << 1) | cpu->C;
    addressing.store(cpu, operand, a);
    cpu_set_zn(cpu, a);
}

void cpu_ror(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = addressing.load(cpu, operand);
    cpu->C = a & 1;
    a = (a >> 1) | (cpu->C << 7);
    addressing.store(cpu, operand, a);
    cpu_set_zn(cpu, a);
}

void cpu_rti(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    cpu_set_status(cpu, cpu_pull(cpu));
    cpu->PC = cpu_pull16(cpu);
}

void cpu_rts(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint16_t address = cpu_pull16(cpu);
    cpu->PC = address;
}

void cpu_sbc(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    uint8_t a = cpu->A;
    uint8_t b = addressing.load(cpu, operand);
    uint8_t c = cpu->C;
    cpu->A = a - b - !c;
    cpu->C = a < b + !c;
    cpu->V = (u8sign(a) == u8sign(b) && u8sign(cpu->A) != u8sign(b));
    cpu_set_zn(cpu, a);
}

void cpu_sec(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->C = 1;
}

void cpu_sed(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->D = 1;
}

void cpu_sei(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->I = 1;
}

void cpu_sta(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    addressing.store(cpu, operand, cpu->A);
}

void cpu_stx(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    addressing.store(cpu, operand, cpu->X);
}

void cpu_sty(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    addressing.store(cpu, operand, cpu->Y);
}

void cpu_tax(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->X = cpu->A;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_tay(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->Y = cpu->A;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_tsx(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->X = cpu->SP;
}

void cpu_txa(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->A = cpu->X;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_tya(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->A = cpu->Y;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_txs(Cpu *cpu, Addressing addressing, uint16_t operand)
{
    UNUSED2(addressing, operand);
    cpu->SP = cpu->X;
}
