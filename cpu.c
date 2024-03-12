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
    [IT_ARR] = NULL, // Illegal
    [IT_ASL] = NULL,
    [IT_AXS] = NULL, // Illegal
    [IT_BIT] = NULL,
    [IT_BRK] = cpu_brk,
    [IT_CLC] = NULL,
    [IT_CLD] = NULL,
    [IT_CLI] = NULL,
    [IT_CLV] = NULL,
    [IT_CMP] = NULL,
    [IT_CPX] = NULL,
    [IT_CPY] = NULL,
    [IT_DCP] = NULL, // Illegal
    [IT_DEC] = NULL,
    [IT_DEX] = NULL,
    [IT_DEY] = NULL,
    [IT_EOR] = NULL,
    [IT_IGN] = NULL, // Illegal
    [IT_INC] = NULL,
    [IT_INX] = cpu_inx,
    [IT_INY] = NULL,
    [IT_ISC] = NULL, // Illegal
    [IT_JMP] = NULL,
    [IT_JSR] = NULL,
    [IT_LAX] = NULL, // Illegal
    [IT_LDA] = cpu_lda,
    [IT_LDX] = NULL,
    [IT_LDY] = NULL,
    [IT_LSR] = NULL,
    [IT_NOP] = NULL,
    [IT_ORA] = NULL,
    [IT_EOL] = NULL,
    [IT_RLA] = NULL, // Illegal
    [IT_RRA] = NULL, // Illegal
    [IT_ROR] = NULL,
    [IT_RTI] = NULL,
    [IT_RTS] = NULL,
    [IT_SAX] = NULL, // Illegal
    [IT_SBC] = NULL,
    [IT_SEC] = NULL,
    [IT_SED] = NULL,
    [IT_SEI] = NULL,
    [IT_SKB] = NULL, // Illegal
    [IT_SLO] = NULL, // Illegal
    [IT_SRE] = NULL, // Illegal
    [IT_STA] = cpu_sta,
    [IT_STX] = NULL,
    [IT_STY] = NULL,
    [IT_TAX] = cpu_tax,
    [IT_TAY] = NULL,
    [IT_TXA] = NULL,
    [IT_TYA] = NULL,
};

const Instruction instructions[256] = {
    [0x69] = {IT_ADC, AM_IMMEDIATE, 2},
    [0xA9] = {IT_LDA, AM_IMMEDIATE, 2},
    [0x8D] = {IT_STA, AM_ABSOLUTE, 5},
    [0xAA] = {IT_TAX, AM_IMPLIED, 2},
    [0xE8] = {IT_INX, AM_IMPLIED, 2},
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
    [IT_EOL] = "EOL",
    [IT_RLA] = "RLA",
    [IT_RRA] = "RRA",
    [IT_ROR] = "ROR",
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
    (void)cpu;
    (void)operand;
    return 0;
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
    (void)cpu;
    (void)ls;
    (void)data;
}

void cpu_inx(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->X++;
    cpu_set_zn(cpu, cpu->X);
}

void cpu_lda(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    uint8_t a = ls.load(cpu, data);
    cpu->A = a;
    cpu_set_zn(cpu, a);
}

void cpu_sta(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    ls.store(cpu, data, cpu->A);
}

void cpu_tax(Cpu *cpu, CpuLoadStore ls, uint16_t data)
{
    (void)ls;
    (void)data;
    cpu->X = cpu->A;
    cpu_set_zn(cpu, cpu->A);
}

bool u8sign(uint8_t value)
{
    return value & 0b10000000;
}
