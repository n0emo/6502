#include "cpu.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    Cpu cpu = {0};
    Memory mem = {0};

    cpu_init(&cpu, &mem);

    // NES Memory map

    // Internal RAM
    mem_append(&mem,
               (Device){
                   .data = calloc(0x0800, 1),
                   .begin_address = 0,
                   .end_address = 0x07FF,
                   .readonly = false,
               });

    // PPU
    mem_append(&mem,
               (Device){
                   .data = calloc(0x2000, 1),
                   .begin_address = 0x2000,
                   .end_address = 0x3FFF,
                   .readonly = false,
               });

    // APU
    mem_append(&mem,
               (Device){
                   .data = calloc(0x2000, 1),
                   .begin_address = 0x4000,
                   .end_address = 0x5FFF,
                   .readonly = false,
               });

    // Cartridge RAM
    mem_append(&mem,
               (Device){
                   .data = calloc(0x2000, 1),
                   .begin_address = 0x6000,
                   .end_address = 0x7FFF,
                   .readonly = false,
               });

    // ROM
    mem_append(&mem,
               (Device){
                   .data = calloc(0x8000, 1),
                   .begin_address = 0x8000,
                   .end_address = 0xFFFA,
                   .readonly = true,
               });

    // Vectors
    mem_append(&mem,
               (Device){
                   .data = calloc(6, 1),
                   .begin_address = 0xFFFA,
                   .end_address = 0xFFFF,
                   .readonly = true,
               });

    mem_write_force(&mem, 0xFFFC, 0x80);
    mem_write_force(&mem, 0xFFFD, 0x00);

    uint8_t init[] = {
        // Init stack
        0xa2, // LDX
        0xff, // #$ff

        0x9a, // TXS

        // Init CPU
        0xa9, // LDA
        0x00, // #$00

        0xa2, // LDX
        0x00, // #$00

        0xc9, // CMP
        0xff, // #$ff

        0x4c, // JMP
        0x00, // $0600
        0x06,
    };

    uint8_t example[] = {
        0x20,
        0x09,
        0x06,
        0x20,
        0x0f,
        0x06,
        0x20,
        0x15,
        0x06,
        0xac,
        0xfe,
        0x01,
        0xa2,
        0x00,
        0x60,
        0xe8,
        0xe0,
        0x05,
        0xd0,
        0xfb,
        0x60,
        0x00,
    };

    for (size_t i = 0; i < sizeof(example) / sizeof(example[0]); i++)
    {
        mem_write_force(&mem, 0x0600 + i, example[i]);
    }

    for (size_t i = 0; i < sizeof(init) / sizeof(init[0]); i++)
    {
        mem_write_force(&mem, 0x8000 + i, init[i]);
    }

    cpu_reset(&cpu);
    printf("Initial state:\n");
    cpu_print(&cpu);

    cpu.B = 1;
    size_t limit = 999;
    while (cpu.B && limit)
    {
        cpu_execute(&cpu);
        limit--;
    }

    return 0;
}
