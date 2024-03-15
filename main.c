#include "cpu.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    Cpu cpu = {0};
    Memory mem = {0};

    cpu_init(&cpu, &mem);

    // NES Memory map

    // Internal RAM
    mem_append(&mem,
               (Device){
                   .data = malloc(0x0800),
                   .begin_address = 0,
                   .end_address = 0x07FF,
                   .readonly = false,
               });

    // PPU
    mem_append(&mem,
               (Device){
                   .data = malloc(0x2000),
                   .begin_address = 0x2000,
                   .end_address = 0x3FFF,
                   .readonly = false,
               });

    // APU
    mem_append(&mem,
               (Device){
                   .data = malloc(0x2000),
                   .begin_address = 0x4000,
                   .end_address = 0x5FFF,
                   .readonly = false,
               });

    // Cartridge RAM
    mem_append(&mem,
               (Device){
                   .data = malloc(0x2000),
                   .begin_address = 0x6000,
                   .end_address = 0x7FFF,
                   .readonly = false,
               });

    // ROM
    mem_append(&mem,
               (Device){
                   .data = malloc(0x8000),
                   .begin_address = 0x8000,
                   .end_address = 0xFFFA,
                   .readonly = true,
               });

    // Vectors
    mem_append(&mem,
               (Device){
                   .data = malloc(6),
                   .begin_address = 0xFFFA,
                   .end_address = 0xFFFF,
                   .readonly = true,
               });

    mem_write_force(&mem, 0xFFFC, 0x06);
    mem_write_force(&mem, 0xFFFD, 0x00);

    uint8_t example[] = {
        // Init stack
        0xa2,
        0xff,
        0x9a,
        // Rest of the program
        0xa9,
        0xc0,
        0xaa,
        0xe8,
        0x69,
        0xc4,
        // 0x00,
    };

    for (size_t i = 0; i < sizeof(example) / sizeof(example[0]); i++)
    {
        mem_write_force(&mem, 0x0600 + i, example[i]);
    }

    cpu_reset(&cpu);
    printf("Initial state:\n");
    cpu_print(&cpu);

    for (size_t i = 0; i < 6; i++)
    {
        cpu_execute(&cpu);
    }

    return 0;
}
