#include "cpu.h"
#include <stdio.h>

const CpuLoadStore load_stores[] = {
    [AM_ABSOLUTE] = {cpu_load_absolute, cpu_store_absolute, 3},
    [AM_ABSOLUTE_X] = {cpu_load_absolute_x, cpu_store_absolute_x, 3},
    [AM_ABSOLUTE_Y] = {cpu_load_absolute_y, cpu_store_absolute_y, 3},
    [AM_ACCUMULATOR] = {cpu_load_accumulator, cpu_store_accumulator, 1},
    [AM_IMMEDIATE] = {cpu_load_immediate, cpu_store_immediate, 2},
    [AM_IMPLIED] = {cpu_load_implied, cpu_store_implied, 1},
    [AM_INDIRECT] = {cpu_load_indirect, cpu_store_indirect, 3},
    [AM_INDIRECT_X] = {cpu_load_indirect_x, cpu_store_absolute_x, 2},
    [AM_INDIRECT_Y] = {cpu_load_indirect_y, cpu_store_indirect_y, 2},
    [AM_RELATIVE] = {cpu_load_relative, cpu_store_relative, 2},
    [AM_ZEROPAGE] = {cpu_load_zeropage, cpu_store_zeropage, 2},
    [AM_ZEROPAGE_X] = {cpu_load_zeropage_x, cpu_store_zeropage_x, 2},
    [AM_ZEROPAGE_Y] = {cpu_load_zeropage_y, cpu_store_zeropage_y, 2},
};

instruction_func_t *const inst_funcs[] = {
    [IT_ADC] = cpu_adc,
    [IT_ALR] = NULL, // Illegal
    [IT_ANC] = NULL, // Illegal
    [IT_AND] = cpu_and,
    [IT_ANE] = NULL,
    [IT_ARR] = NULL, // Illegal
    [IT_ASL] = cpu_asl,
    [IT_AXS] = NULL, // Illegal
    [IT_BCC] = NULL,
    [IT_BCS] = NULL,
    [IT_BEQ] = NULL,
    [IT_BIT] = NULL,
    [IT_BMI] = NULL,
    [IT_BNE] = NULL,
    [IT_BRK] = cpu_brk,
    [IT_BVC] = NULL,
    [IT_BVS] = NULL,
    [IT_CLC] = cpu_clc,
    [IT_CLD] = cpu_cld,
    [IT_CLI] = cpu_cli,
    [IT_CLV] = cpu_clv,
    [IT_CMP] = NULL,
    [IT_CPX] = NULL,
    [IT_CPY] = NULL,
    [IT_DCP] = NULL, // Illegal
    [IT_DEC] = NULL,
    [IT_DEX] = NULL,
    [IT_DEY] = NULL,
    [IT_EOR] = NULL,
    [IT_IGN] = NULL, // Illegal
    [IT_INC] = cpu_inc,
    [IT_INX] = cpu_inx,
    [IT_INY] = cpu_iny,
    [IT_ISC] = NULL, // Illegal
    [IT_JAM] = NULL, // Illegal
    [IT_JMP] = NULL,
    [IT_JSR] = NULL,
    [IT_LAX] = NULL, // Illegal
    [IT_LDA] = cpu_lda,
    [IT_LDX] = cpu_ldx,
    [IT_LDY] = cpu_ldy,
    [IT_LSR] = NULL,
    [IT_LXA] = NULL, // Illegal
    [IT_NOP] = NULL,
    [IT_ORA] = NULL,
    [IT_PHA] = NULL,
    [IT_PHP] = NULL,
    [IT_PLA] = NULL,
    [IT_PLP] = NULL,
    [IT_RLA] = NULL, // Illegal
    [IT_ROL] = NULL,
    [IT_ROR] = NULL,
    [IT_RRA] = NULL, // Illegal
    [IT_RTI] = NULL,
    [IT_RTS] = NULL,
    [IT_SAX] = NULL, // Illegal
    [IT_SBC] = NULL,
    [IT_SBX] = NULL, // Illegal
    [IT_SEC] = NULL,
    [IT_SED] = NULL,
    [IT_SEI] = NULL,
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
    [IT_TSX] = NULL,
    [IT_TXA] = cpu_txa,
    [IT_TXS] = NULL,
    [IT_TYA] = cpu_tya,
    [IT_USBC] = NULL,
};

const Instruction instructions[256] = {
    [0x00] = {IT_BRK, AM_IMPLIED, 7},
    [0x01] = {IT_ORA, AM_INDIRECT_X, 6},
    [0x02] = {IT_JAM, AM_IMPLIED, 2},
    [0x03] = {IT_SLO, AM_INDIRECT_X, 8},
    [0x04] = {IT_NOP, AM_ZEROPAGE, 3},
    [0x05] = {IT_ORA, AM_ZEROPAGE, 3},
    [0x06] = {IT_ASL, AM_ZEROPAGE, 5},
    [0x07] = {IT_SLO, AM_ZEROPAGE, 5},
    [0x08] = {IT_PHP, AM_IMPLIED, 3},
    [0x09] = {IT_ORA, AM_IMMEDIATE, 2},
    [0x0A] = {IT_ASL, AM_ACCUMULATOR, 2},
    [0x0B] = {IT_ANC, AM_IMMEDIATE, 2},
    [0x0C] = {IT_NOP, AM_ABSOLUTE, 4},
    [0x0D] = {IT_ORA, AM_ABSOLUTE, 4},
    [0x0E] = {IT_ASL, AM_ABSOLUTE, 6},
    [0x0F] = {IT_SLO, AM_ABSOLUTE, 6},

    [0x10] = {IT_BRK, AM_RELATIVE, 2},
    [0x11] = {IT_ORA, AM_INDIRECT_Y, 5},
    [0x12] = {IT_JAM, AM_IMPLIED, 2},
    [0x13] = {IT_SLO, AM_INDIRECT_Y, 8},
    [0x14] = {IT_NOP, AM_ZEROPAGE, 4},
    [0x15] = {IT_ORA, AM_ZEROPAGE_X, 4},
    [0x16] = {IT_ASL, AM_ZEROPAGE_X, 6},
    [0x17] = {IT_SLO, AM_ZEROPAGE_X, 6},
    [0x18] = {IT_CLC, AM_IMPLIED, 2},
    [0x19] = {IT_ORA, AM_ABSOLUTE_Y, 4},
    [0x1A] = {IT_NOP, AM_IMPLIED, 2},
    [0x1B] = {IT_SLO, AM_ABSOLUTE_Y, 7},
    [0x1C] = {IT_NOP, AM_ABSOLUTE_X, 4},
    [0x1D] = {IT_ORA, AM_ABSOLUTE_X, 4},
    [0x1E] = {IT_ASL, AM_ABSOLUTE_X, 7},
    [0x1F] = {IT_SLO, AM_ABSOLUTE_X, 7},

    [0x20] = {IT_JSR, AM_ABSOLUTE, 6},
    [0x21] = {IT_AND, AM_INDIRECT_X, 6},
    [0x22] = {IT_JAM, AM_IMPLIED, 2},
    [0x23] = {IT_RLA, AM_INDIRECT_X, 8},
    [0x24] = {IT_BIT, AM_ZEROPAGE, 3},
    [0x25] = {IT_AND, AM_ZEROPAGE, 3},
    [0x26] = {IT_ROL, AM_ZEROPAGE, 5},
    [0x27] = {IT_RLA, AM_ZEROPAGE, 5},
    [0x28] = {IT_PLP, AM_IMPLIED, 4},
    [0x29] = {IT_AND, AM_IMMEDIATE, 2},
    [0x2A] = {IT_ROL, AM_ACCUMULATOR, 2},
    [0x2B] = {IT_ANC, AM_IMMEDIATE, 2},
    [0x2C] = {IT_BIT, AM_ABSOLUTE, 4},
    [0x2D] = {IT_AND, AM_ABSOLUTE, 4},
    [0x2E] = {IT_ROL, AM_ABSOLUTE, 6},
    [0x2F] = {IT_RLA, AM_ABSOLUTE, 6},

    [0x30] = {IT_BMI, AM_RELATIVE, 2},
    [0x31] = {IT_AND, AM_INDIRECT_Y, 5},
    [0x32] = {IT_JAM, AM_IMPLIED, 2},
    [0x33] = {IT_RLA, AM_INDIRECT_Y, 8},
    [0x34] = {IT_NOP, AM_ZEROPAGE_X, 4},
    [0x35] = {IT_AND, AM_ZEROPAGE_X, 4},
    [0x36] = {IT_ROL, AM_ZEROPAGE_X, 6},
    [0x37] = {IT_RLA, AM_ZEROPAGE_X, 6},
    [0x38] = {IT_SEC, AM_IMPLIED, 2},
    [0x39] = {IT_AND, AM_ABSOLUTE_Y, 4},
    [0x3A] = {IT_NOP, AM_IMPLIED, 2},
    [0x3B] = {IT_RLA, AM_ABSOLUTE_Y, 7},
    [0x3C] = {IT_NOP, AM_ABSOLUTE_X, 4},
    [0x3D] = {IT_AND, AM_ABSOLUTE_X, 4},
    [0x3E] = {IT_ROL, AM_ABSOLUTE_X, 7},
    [0x3F] = {IT_RLA, AM_ABSOLUTE_X, 7},

    [0x40] = {IT_RTI, AM_IMPLIED, 6},
    [0x41] = {IT_EOR, AM_INDIRECT_X, 6},
    [0x42] = {IT_JAM, AM_IMPLIED, 2},
    [0x43] = {IT_SRE, AM_INDIRECT_X, 8},
    [0x44] = {IT_NOP, AM_ZEROPAGE, 3},
    [0x45] = {IT_EOR, AM_ZEROPAGE, 3},
    [0x46] = {IT_LSR, AM_ZEROPAGE, 5},
    [0x47] = {IT_SRE, AM_ZEROPAGE, 5},
    [0x48] = {IT_PHA, AM_IMPLIED, 3},
    [0x49] = {IT_EOR, AM_IMMEDIATE, 2},
    [0x4A] = {IT_LSR, AM_ACCUMULATOR, 2},
    [0x4B] = {IT_ALR, AM_IMMEDIATE, 2},
    [0x4C] = {IT_JMP, AM_ABSOLUTE, 3},
    [0x4D] = {IT_EOR, AM_ABSOLUTE, 4},
    [0x4E] = {IT_LSR, AM_ABSOLUTE, 6},
    [0x4F] = {IT_SRE, AM_ABSOLUTE, 6},

    [0x50] = {IT_BVC, AM_RELATIVE, 2},
    [0x51] = {IT_EOR, AM_INDIRECT_Y, 5},
    [0x52] = {IT_JAM, AM_IMPLIED, 2},
    [0x53] = {IT_SRE, AM_INDIRECT_Y, 8},
    [0x54] = {IT_NOP, AM_ZEROPAGE_X, 4},
    [0x55] = {IT_EOR, AM_ZEROPAGE_X, 4},
    [0x56] = {IT_LSR, AM_ZEROPAGE_X, 6},
    [0x57] = {IT_SRE, AM_ZEROPAGE_X, 6},
    [0x58] = {IT_CLI, AM_IMPLIED, 2},
    [0x59] = {IT_EOR, AM_ABSOLUTE_Y, 4},
    [0x5A] = {IT_NOP, AM_IMPLIED, 2},
    [0x5B] = {IT_SRE, AM_ABSOLUTE_Y, 7},
    [0x5C] = {IT_NOP, AM_ABSOLUTE_X, 4},
    [0x5D] = {IT_EOR, AM_ABSOLUTE_X, 4},
    [0x5E] = {IT_LSR, AM_ABSOLUTE_X, 7},
    [0x5F] = {IT_SRE, AM_ABSOLUTE_X, 7},

    [0x60] = {IT_RTS, AM_IMPLIED, 6},
    [0x61] = {IT_ADC, AM_INDIRECT_X, 6},
    [0x62] = {IT_JAM, AM_IMPLIED, 2},
    [0x63] = {IT_RRA, AM_INDIRECT_X, 8},
    [0x64] = {IT_NOP, AM_ZEROPAGE, 3},
    [0x65] = {IT_ADC, AM_ZEROPAGE, 3},
    [0x66] = {IT_ROR, AM_ZEROPAGE, 5},
    [0x67] = {IT_RRA, AM_ZEROPAGE, 5},
    [0x68] = {IT_PLA, AM_IMPLIED, 4},
    [0x69] = {IT_ADC, AM_IMMEDIATE, 2},
    [0x6A] = {IT_ROR, AM_ACCUMULATOR, 2},
    [0x6B] = {IT_ARR, AM_IMMEDIATE, 2},
    [0x6C] = {IT_JMP, AM_INDIRECT, 5},
    [0x6D] = {IT_ADC, AM_ABSOLUTE, 4},
    [0x6E] = {IT_ROR, AM_ABSOLUTE, 6},
    [0x6F] = {IT_RRA, AM_ABSOLUTE, 6},

    [0x70] = {IT_BVS, AM_RELATIVE, 2},
    [0x71] = {IT_ADC, AM_INDIRECT_Y, 5},
    [0x72] = {IT_JAM, AM_IMPLIED, 2},
    [0x73] = {IT_RRA, AM_INDIRECT_Y, 8},
    [0x74] = {IT_NOP, AM_ZEROPAGE_X, 4},
    [0x75] = {IT_ADC, AM_ZEROPAGE_X, 4},
    [0x76] = {IT_ROR, AM_ZEROPAGE_X, 6},
    [0x77] = {IT_RRA, AM_ZEROPAGE_X, 6},
    [0x78] = {IT_SEI, AM_IMPLIED, 2},
    [0x79] = {IT_ADC, AM_ABSOLUTE_Y, 4},
    [0x7A] = {IT_NOP, AM_IMPLIED, 2},
    [0x7B] = {IT_RRA, AM_ABSOLUTE_Y, 7},
    [0x7C] = {IT_NOP, AM_ABSOLUTE_X, 4},
    [0x7D] = {IT_ADC, AM_ABSOLUTE_X, 4},
    [0x7E] = {IT_ROR, AM_ABSOLUTE_X, 7},
    [0x7F] = {IT_RRA, AM_ABSOLUTE_X, 7},

    [0x80] = {IT_NOP, AM_IMMEDIATE, 2},
    [0x81] = {IT_STA, AM_INDIRECT_X, 6},
    [0x82] = {IT_NOP, AM_IMMEDIATE, 2},
    [0x83] = {IT_SAX, AM_INDIRECT_X, 6},
    [0x84] = {IT_STY, AM_ZEROPAGE, 3},
    [0x85] = {IT_STA, AM_ZEROPAGE, 3},
    [0x86] = {IT_STX, AM_ZEROPAGE, 3},
    [0x87] = {IT_SAX, AM_ZEROPAGE, 3},
    [0x88] = {IT_DEY, AM_IMPLIED, 2},
    [0x89] = {IT_NOP, AM_IMMEDIATE, 2},
    [0x8A] = {IT_TXA, AM_IMPLIED, 2},
    [0x8B] = {IT_ANE, AM_IMMEDIATE, 2},
    [0x8C] = {IT_STY, AM_ABSOLUTE, 4},
    [0x8D] = {IT_STA, AM_ABSOLUTE, 4},
    [0x8E] = {IT_STX, AM_ABSOLUTE, 4},
    [0x8F] = {IT_SAX, AM_ABSOLUTE, 4},

    [0x90] = {IT_BCC, AM_RELATIVE, 2},
    [0x91] = {IT_STA, AM_INDIRECT_Y, 6},
    [0x92] = {IT_JAM, AM_IMPLIED, 2},
    [0x93] = {IT_SHA, AM_INDIRECT_Y, 6},
    [0x94] = {IT_STY, AM_ZEROPAGE_X, 4},
    [0x95] = {IT_STA, AM_ZEROPAGE_X, 4},
    [0x96] = {IT_STX, AM_ZEROPAGE_Y, 4},
    [0x97] = {IT_SAX, AM_ZEROPAGE_Y, 4},
    [0x98] = {IT_TYA, AM_IMPLIED, 2},
    [0x99] = {IT_STA, AM_ABSOLUTE_Y, 5},
    [0x9A] = {IT_TXS, AM_IMPLIED, 2},
    [0x9B] = {IT_TAS, AM_ABSOLUTE_Y, 5},
    [0x9C] = {IT_SHY, AM_ABSOLUTE_X, 5},
    [0x9D] = {IT_STA, AM_ABSOLUTE_X, 5},
    [0x9E] = {IT_SHX, AM_ABSOLUTE_Y, 5},
    [0x9F] = {IT_SHA, AM_ABSOLUTE_Y, 5},

    [0xA0] = {IT_LDY, AM_IMMEDIATE, 2},
    [0xA1] = {IT_LDA, AM_INDIRECT_X, 6},
    [0xA2] = {IT_LDX, AM_IMMEDIATE, 2},
    [0xA3] = {IT_LAX, AM_INDIRECT_X, 6},
    [0xA4] = {IT_LDY, AM_ZEROPAGE, 3},
    [0xA5] = {IT_LDA, AM_ZEROPAGE, 3},
    [0xA6] = {IT_LDX, AM_ZEROPAGE, 3},
    [0xA7] = {IT_LAX, AM_ZEROPAGE, 3},
    [0xA8] = {IT_TAY, AM_IMPLIED, 2},
    [0xA9] = {IT_LDA, AM_IMMEDIATE, 2},
    [0xAA] = {IT_TAX, AM_IMPLIED, 2},
    [0xAB] = {IT_LXA, AM_IMMEDIATE, 2},
    [0xAC] = {IT_LDY, AM_ABSOLUTE, 4},
    [0xAD] = {IT_LDA, AM_ABSOLUTE, 4},
    [0xAE] = {IT_LDX, AM_ABSOLUTE, 4},
    [0xAF] = {IT_LAX, AM_ABSOLUTE, 4},

    [0xB0] = {IT_BCS, AM_RELATIVE, 2},
    [0xB1] = {IT_LDA, AM_INDIRECT_Y, 5},
    [0xB2] = {IT_JAM, AM_IMPLIED, 2},
    [0xB3] = {IT_LAX, AM_INDIRECT_Y, 5},
    [0xB4] = {IT_LDY, AM_INDIRECT_X, 4},
    [0xB5] = {IT_LDA, AM_ZEROPAGE_X, 4},
    [0xB6] = {IT_LDX, AM_ZEROPAGE_Y, 4},
    [0xB7] = {IT_LAX, AM_ZEROPAGE_Y, 4},
    [0xB8] = {IT_CLV, AM_IMPLIED, 2},
    [0xB9] = {IT_LDA, AM_ABSOLUTE_Y, 4},
    [0xBA] = {IT_TSX, AM_IMPLIED, 2},
    [0xBB] = {IT_LAX, AM_ABSOLUTE_Y, 4},
    [0xBC] = {IT_LDY, AM_ABSOLUTE_X, 4},
    [0xBD] = {IT_LDA, AM_ABSOLUTE_X, 4},
    [0xBE] = {IT_LDX, AM_ABSOLUTE_Y, 4},
    [0xBF] = {IT_LAX, AM_ABSOLUTE_Y, 4},

    [0xC0] = {IT_CPY, AM_IMMEDIATE, 2},
    [0xC1] = {IT_CMP, AM_INDIRECT_X, 6},
    [0xC2] = {IT_NOP, AM_IMMEDIATE, 2},
    [0xC3] = {IT_DCP, AM_INDIRECT_X, 8},
    [0xC4] = {IT_CPY, AM_ZEROPAGE, 3},
    [0xC5] = {IT_CMP, AM_ZEROPAGE, 3},
    [0xC6] = {IT_DEC, AM_ZEROPAGE, 5},
    [0xC7] = {IT_DCP, AM_ZEROPAGE, 5},
    [0xC8] = {IT_INY, AM_IMPLIED, 2},
    [0xC9] = {IT_CMP, AM_IMMEDIATE, 2},
    [0xCA] = {IT_DEX, AM_IMPLIED, 2},
    [0xCB] = {IT_SBX, AM_IMMEDIATE, 2},
    [0xCC] = {IT_CPY, AM_ABSOLUTE, 4},
    [0xCD] = {IT_CMP, AM_ABSOLUTE, 4},
    [0xCE] = {IT_DEC, AM_ABSOLUTE, 6},
    [0xCF] = {IT_DCP, AM_ABSOLUTE, 6},

    [0xD0] = {IT_BNE, AM_RELATIVE, 2},
    [0xD1] = {IT_CMP, AM_INDIRECT_Y, 5},
    [0xD2] = {IT_JAM, AM_IMPLIED, 2},
    [0xD3] = {IT_DCP, AM_INDIRECT_Y, 8},
    [0xD4] = {IT_NOP, AM_ZEROPAGE_X, 4},
    [0xD5] = {IT_CMP, AM_ZEROPAGE_X, 4},
    [0xD6] = {IT_DEC, AM_ZEROPAGE_X, 6},
    [0xD7] = {IT_DCP, AM_ZEROPAGE_X, 6},
    [0xD8] = {IT_CLD, AM_IMPLIED, 2},
    [0xD9] = {IT_CMP, AM_ABSOLUTE_Y, 4},
    [0xDA] = {IT_NOP, AM_IMPLIED, 2},
    [0xDB] = {IT_DCP, AM_ABSOLUTE_X, 7},
    [0xDC] = {IT_NOP, AM_ABSOLUTE_X, 4},
    [0xDD] = {IT_CMP, AM_ABSOLUTE_X, 4},
    [0xDE] = {IT_DEC, AM_ABSOLUTE_X, 7},
    [0xDF] = {IT_DCP, AM_ABSOLUTE_X, 7},

    [0xE0] = {IT_CPX, AM_IMMEDIATE, 2},
    [0xE1] = {IT_SBC, AM_INDIRECT_X, 6},
    [0xE2] = {IT_NOP, AM_IMMEDIATE, 2},
    [0xE3] = {IT_ISC, AM_INDIRECT_X, 8},
    [0xE4] = {IT_CPX, AM_ZEROPAGE, 3},
    [0xE5] = {IT_SBC, AM_ZEROPAGE, 3},
    [0xE6] = {IT_INC, AM_ZEROPAGE, 5},
    [0xE7] = {IT_ISC, AM_ZEROPAGE, 5},
    [0xE8] = {IT_INX, AM_IMPLIED, 2},
    [0xE9] = {IT_SBC, AM_IMMEDIATE, 2},
    [0xEA] = {IT_NOP, AM_IMPLIED, 2},
    [0xEB] = {IT_USBC, AM_IMPLIED, 2},
    [0xEC] = {IT_CPX, AM_ABSOLUTE, 4},
    [0xED] = {IT_SBC, AM_ABSOLUTE, 4},
    [0xEE] = {IT_INC, AM_ABSOLUTE, 6},
    [0xEF] = {IT_ISC, AM_ABSOLUTE, 6},

    [0xF0] = {IT_BEQ, AM_RELATIVE, 2},
    [0xF1] = {IT_SBC, AM_INDIRECT_Y, 5},
    [0xF2] = {IT_JAM, AM_IMPLIED, 2},
    [0xF3] = {IT_ISC, AM_INDIRECT_Y, 8},
    [0xF4] = {IT_NOP, AM_ZEROPAGE_X, 4},
    [0xF5] = {IT_SBC, AM_ZEROPAGE_X, 4},
    [0xF6] = {IT_INC, AM_ZEROPAGE_X, 6},
    [0xF7] = {IT_ISC, AM_ZEROPAGE_X, 6},
    [0xF8] = {IT_SED, AM_IMPLIED, 2},
    [0xF9] = {IT_SBC, AM_ABSOLUTE_Y, 4},
    [0xFA] = {IT_NOP, AM_IMPLIED, 2},
    [0xFB] = {IT_ISC, AM_ABSOLUTE_Y, 7},
    [0xFC] = {IT_NOP, AM_ABSOLUTE_X, 4},
    [0xFD] = {IT_SBC, AM_ABSOLUTE_X, 4},
    [0xFE] = {IT_INC, AM_ABSOLUTE_X, 7},
    [0xFF] = {IT_ISC, AM_ABSOLUTE_X, 7},
};

const char *cpu_inst_names[] = {
    [IT_ADC] = "ADC",
    [IT_ALR] = "ALR",
    [IT_ANC] = "ANC",
    [IT_AND] = "AND",
    [IT_ARR] = "ARR",
    [IT_ASL] = "ASL",
    [IT_AXS] = "AXS",
    [IT_BIT] = "BIT",
    [IT_BRK] = "BRK",
    [IT_CLC] = "CLC",
    [IT_CLD] = "CLD",
    [IT_CLI] = "CLI",
    [IT_CLV] = "CLV",
    [IT_CMP] = "CMP",
    [IT_CPX] = "CPX",
    [IT_CPY] = "CPY",
    [IT_DCP] = "DCP",
    [IT_DEC] = "DEC",
    [IT_DEX] = "DEX",
    [IT_DEY] = "DEY",
    [IT_EOR] = "EOR",
    [IT_IGN] = "IGN",
    [IT_INC] = "INC",
    [IT_INX] = "INX",
    [IT_INY] = "INY",
    [IT_ISC] = "ISC",
    [IT_JMP] = "JMP",
    [IT_JSR] = "JSR",
    [IT_LAX] = "LAX",
    [IT_LDA] = "LDA",
    [IT_LDX] = "LDX",
    [IT_LDY] = "LDY",
    [IT_LSR] = "LSR",
    [IT_NOP] = "NOP",
    [IT_ORA] = "ORA",
    [IT_RLA] = "RLA",
    [IT_ROR] = "ROR",
    [IT_RRA] = "RRA",
    [IT_RTI] = "RTI",
    [IT_RTS] = "RTS",
    [IT_SAX] = "SAX",
    [IT_SBC] = "SBC",
    [IT_SEC] = "SEC",
    [IT_SED] = "SED",
    [IT_SEI] = "SEI",
    [IT_SKB] = "SKB",
    [IT_SLO] = "SLO",
    [IT_SRE] = "SRE",
    [IT_STA] = "STA",
    [IT_STX] = "STX",
    [IT_STY] = "STY",
    [IT_TAX] = "TAX",
    [IT_TAY] = "TAY",
    [IT_TXA] = "TXA",
    [IT_TYA] = "TYA",
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
    uint8_t pc_fst = cpu->mem->data[CPU_RESET_VECTOR_1];
    uint8_t pc_snd = cpu->mem->data[CPU_RESET_VECTOR_2];
    cpu->PC = (pc_fst << 8) | pc_snd;
}

void cpu_execute(Cpu *cpu)
{
    uint8_t inst_code = cpu->mem->data[cpu->PC];
    Instruction instruction = instructions[inst_code];
    printf("Executing %s\n", cpu_inst_names[instruction.type]);
    size_t inst_size = load_stores[instruction.address_mode].size;
    uint16_t data = cpu_read16(cpu, cpu->PC + 1);
    inst_funcs[instruction.type](cpu, load_stores[instruction.address_mode], data);
    cpu->PC += inst_size;
    cpu_print(cpu);
}

uint16_t cpu_read16(Cpu *cpu, uint16_t address)
{
    uint8_t lo = cpu->mem->data[address];
    uint8_t hi = cpu->mem->data[address + 1];
    return u16_from_lohi(lo, hi);
}

uint8_t u16_lo(uint16_t u16)
{
    return u16 & 0x00FF;
}

uint8_t u16_hi(uint16_t u16)
{
    return u16 & 0xFF00 >> 8;
}

uint16_t u16_from_lohi(uint8_t lo, uint8_t hi)
{
    return lo | (hi << 8);
}

uint8_t cpu_load_absolute(Cpu *cpu, uint16_t operand)
{
    return cpu->mem->data[operand];
}
uint8_t cpu_load_absolute_x(Cpu *cpu, uint16_t operand)
{
    return cpu->mem->data[operand + cpu->X];
}

uint8_t cpu_load_absolute_y(Cpu *cpu, uint16_t operand)
{
    return cpu->mem->data[operand + cpu->Y];
}

uint8_t cpu_load_accumulator(Cpu *cpu, uint16_t operand)
{
    (void)operand;
    return cpu->A;
}

uint8_t cpu_load_immediate(Cpu *cpu, uint16_t operand)
{
    (void)cpu;
    return u16_lo(operand);
}

uint8_t cpu_load_implied(Cpu *cpu, uint16_t operand)
{
    (void)cpu;
    (void)operand;
    return 0;
}

uint8_t cpu_load_indirect(Cpu *cpu, uint16_t operand)
{
    uint16_t address = cpu_read16(cpu, operand);
    return cpu->mem->data[address];
}

uint8_t cpu_load_indirect_x(Cpu *cpu, uint16_t operand)
{
    uint8_t address_to_address = operand + cpu->X;
    uint8_t address = u16_lo(cpu_read16(cpu, address_to_address));
    return cpu->mem->data[address];
}

uint8_t cpu_load_indirect_y(Cpu *cpu, uint16_t operand)
{
    uint8_t address = u16_lo(cpu_read16(cpu, operand)) + cpu->Y;
    return cpu->mem->data[address];
}

uint8_t cpu_load_relative(Cpu *cpu, uint16_t operand)
{
    return cpu->PC + u16_lo(operand);
}

uint8_t cpu_load_zeropage(Cpu *cpu, uint16_t operand)
{
    uint8_t address = u16_lo(operand);
    return cpu->mem->data[address];
}

uint8_t cpu_load_zeropage_x(Cpu *cpu, uint16_t operand)
{
    uint8_t address = u16_lo(operand) + cpu->X;
    return cpu->mem->data[address];
}

uint8_t cpu_load_zeropage_y(Cpu *cpu, uint16_t operand)
{
    uint8_t address = u16_lo(operand) + cpu->Y;
    return cpu->mem->data[address];
}

void cpu_store_absolute(Cpu *cpu, uint16_t operand, uint8_t value)
{
    cpu->mem->data[operand] = value;
}

void cpu_store_absolute_x(Cpu *cpu, uint16_t operand, uint8_t value)
{
    cpu->mem->data[operand + cpu->X] = value;
}

void cpu_store_absolute_y(Cpu *cpu, uint16_t operand, uint8_t value)
{
    cpu->mem->data[operand + cpu->Y] = value;
}

void cpu_store_accumulator(Cpu *cpu, uint16_t operand, uint8_t value)
{
    (void)operand;
    cpu->A = value;
}

void cpu_store_immediate(Cpu *cpu, uint16_t operand, uint8_t value)
{
    (void)cpu;
    (void)operand;
    (void)value;
}

void cpu_store_implied(Cpu *cpu, uint16_t operand, uint8_t value)
{
    (void)cpu;
    (void)operand;
    (void)value;
}

void cpu_store_indirect(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint16_t address = cpu_read16(cpu, operand);
    cpu->mem->data[address] = value;
}

void cpu_store_indirect_x(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint8_t address_to_address = operand + cpu->X;
    uint8_t address = u16_lo(cpu_read16(cpu, address_to_address));
    cpu->mem->data[address] = value;
}

void cpu_store_indirect_y(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint8_t address = u16_lo(cpu_read16(cpu, operand)) + cpu->Y;
    cpu->mem->data[address] = value;
}

void cpu_store_relative(Cpu *cpu, uint16_t operand, uint8_t value)
{
    (void)cpu;
    (void)operand;
    (void)value;
}

void cpu_store_zeropage(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint8_t address = u16_lo(operand);
    cpu->mem->data[address] = value;
}

void cpu_store_zeropage_x(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint8_t address = u16_lo(operand) + cpu->Y;
    cpu->mem->data[address] = value;
}

void cpu_store_zeropage_y(Cpu *cpu, uint16_t operand, uint8_t value)
{
    uint8_t address = u16_lo(operand) + cpu->Y;
    cpu->mem->data[address] = value;
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

void cpu_adc(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    uint8_t a = cpu->A;
    uint8_t b = ls.load(cpu, data);
    bool c = cpu->C;
    cpu->A = a + b + c;
    cpu_set_zn(cpu, cpu->A);
    cpu->C = a + b + c > 0xFF;
    cpu->V = (u8sign(a) == u8sign(b) && u8sign(cpu->A) != u8sign(b));
}

void cpu_and(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    uint8_t b = ls.load(cpu, data);
    cpu->A = cpu->A & b;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_asl(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    size_t a = ls.load(cpu, data);
    a = a << 1;
}

void cpu_brk(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    cpu->B = 0;
    (void)ls;
    (void)data;
}

void cpu_clc(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->C = 0;
}

void cpu_cld(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->D = 0;
}

void cpu_cli(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->I = 0;
}

void cpu_clv(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->V = 0;
}

void cpu_inc(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->A++;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_inx(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->X++;
    cpu_set_zn(cpu, cpu->X);
}

void cpu_iny(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->Y++;
    cpu_set_zn(cpu, cpu->Y);
}

void cpu_lda(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    cpu->A = ls.load(cpu, data);
    cpu_set_zn(cpu, cpu->A);
}

void cpu_ldx(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    cpu->X = ls.load(cpu, data);
    cpu_set_zn(cpu, cpu->X);
}

void cpu_ldy(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    cpu->Y = ls.load(cpu, data);
    cpu_set_zn(cpu, cpu->Y);
}

void cpu_nop(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)cpu;
    (void)ls;
    (void)data;
}

void cpu_sta(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    ls.store(cpu, data, cpu->A);
}

void cpu_stx(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    ls.store(cpu, data, cpu->X);
}

void cpu_sty(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    ls.store(cpu, data, cpu->Y);
}

void cpu_tax(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->X = cpu->A;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_tay(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->Y = cpu->A;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_txa(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->A = cpu->X;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_tya(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->A = cpu->Y;
    cpu_set_zn(cpu, cpu->A);
}

bool u8sign(uint8_t value)
{
    return value & 0x80;
}
