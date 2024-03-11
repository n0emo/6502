#include "cpu.h"
#include "mem.h"
#include <stdio.h>
#include <string.h>

int main()
{
    Cpu cpu;
    Memory mem;

    mem_init(&mem);
    cpu_init(&cpu, &mem);

    mem.data[0xFFFC] = 0x06;
    mem.data[0xFFFD] = 0x00;

    uint8_t example[] = {
        0xa9,
        0xc0,
        0xaa,
        0xe8,
        0x69,
        0xc4,
        // 0x00,
    };

    memcpy(mem.data + 0x0600, example, sizeof(example));

    cpu_reset(&cpu);
    printf("Initial state:\n");
    cpu_print(&cpu);

    for (size_t i = 0; i < 4; i++)
    {
        cpu_execute(&cpu);
    }

    return 0;
}
