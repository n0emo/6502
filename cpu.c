#include "cpu.h"
#include <stdio.h>

const Instruction instructions[256] = {
    [0x69] = {"ADC", cpu_adc, AM_IMMEDIATE, 2, 2},
    [0xA9] = {"LDA", cpu_lda, AM_IMMEDIATE, 2, 2},
    [0x8D] = {"STA", cpu_sta, AM_ABSOLUTE, 5, 3},
    [0xAA] = {"TAX", cpu_tax, AM_IMPLIED, 2, 1},
    [0xE8] = {"INX", cpu_inx, AM_IMPLIED, 2, 1},
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
    printf("Executing %s\n", instruction.name);
    size_t data_size = instruction.size - 1;
    uint8_t data[data_size];
    for (size_t i = 0; i < data_size; i++)
    {
        data[i] = cpu->mem->data[i + cpu->PC + 1];
    }
    instruction.func(cpu, instruction.address_mode, data);
    cpu->PC += instruction.size;
    cpu_print(cpu);
}

uint8_t cpu_load(Cpu *cpu, AddressMode mode, uint8_t *data)
{
    switch (mode)
    {
    case AM_IMMEDIATE:
        return data[0];
    default:
        return 0;
    }
}

void cpu_store(Cpu *cpu, uint8_t value, AddressMode mode, uint8_t *data)
{
    switch (mode)
    {
    case AM_ABSOLUTE: {
        uint16_t address = data[0] | (data[1] << 8);
        cpu->mem->data[address] = value;
    }
    break;
    default:
        break;
    }
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

void cpu_adc(Cpu *cpu, AddressMode mode, uint8_t *data)
{
    uint8_t a = cpu->A;
    uint8_t b = cpu_load(cpu, mode, data);
    bool c = cpu->C;
    cpu->A = a + b + c;
    cpu_set_zn(cpu, cpu->A);
    cpu->C = a + b + c > 0xFF;
    cpu->V = (u8sign(a) == u8sign(b) && u8sign(cpu->A) != u8sign(b));
}

void cpu_lda(Cpu *cpu, AddressMode mode, uint8_t *data)
{
    uint8_t a = cpu_load(cpu, mode, data);
    cpu->A = a;
    cpu_set_zn(cpu, a);
}

void cpu_sta(Cpu *cpu, AddressMode mode, uint8_t *data)
{
    cpu_store(cpu, cpu->A, mode, data);
}

void cpu_tax(Cpu *cpu, AddressMode mode, uint8_t *data)
{
    cpu->X = cpu->A;
    cpu_set_zn(cpu, cpu->A);
}

void cpu_inx(Cpu *cpu, AddressMode mode, uint8_t *data)
{
    cpu->X++;
    cpu_set_zn(cpu, cpu->X);
}

bool u8sign(uint8_t value)
{
    return value & 0b10000000;
}
