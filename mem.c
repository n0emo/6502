#include "mem.h"

#include <assert.h>
#include <stdlib.h>

#include "int.h"

#define DEVICE_LIMIT 32

void mem_append(Memory *mem, Device dev)
{
    if (mem->devices == NULL)
    {
        mem->devices = malloc(sizeof(Device) * 16);
        mem->dev_count = 0;
        mem->dev_capacity = 16;
    }
    if (mem->dev_count == mem->dev_capacity)
    {
        size_t new_cap = mem->dev_capacity * 2;
        mem->devices = realloc(mem->devices, sizeof(Device) * new_cap);
        mem->dev_capacity = new_cap;
    }

    mem->devices[mem->dev_count] = dev;
    mem->dev_count++;
}

uint8_t mem_read(Memory *mem, uint16_t address)
{
    for (size_t i = 0; i < mem->dev_count; i++)
    {
        Device dev = mem->devices[i];
        if (address >= dev.begin_address && address < dev.end_address)
        {
            return dev.data[address - dev.begin_address];
        }
    }

    assert(0 && "No device mapped for address.");
}

uint16_t mem_read16(Memory *mem, uint16_t address)
{
    for (size_t i = 0; i < mem->dev_count; i++)
    {
        Device dev = mem->devices[i];
        if (address >= dev.begin_address && address <= dev.end_address)
        {
            uint8_t lo = dev.data[address - dev.begin_address];
            uint8_t hi = dev.data[address - dev.begin_address + 1];
            return u16_from_lohi(lo, hi);
        }
    }

    assert(0 && "No device mapped for address.");
}

void mem_write(Memory *mem, uint16_t address, uint8_t value)
{
    for (size_t i = 0; i < mem->dev_count; i++)
    {
        Device dev = mem->devices[i];
        if (address >= dev.begin_address && address <= dev.end_address)
        {
            if (!dev.readonly)
            {
                dev.data[address - dev.begin_address] = value;
            }
            return;
        }
    }

    assert(0 && "No device mapped for address.");
}

void mem_write_force(Memory *mem, uint16_t address, uint8_t value)
{
    for (size_t i = 0; i < mem->dev_count; i++)
    {
        Device dev = mem->devices[i];
        if (address >= dev.begin_address && address <= dev.end_address)
        {
            dev.data[address - dev.begin_address] = value;
            return;
        }
    }

    assert(0 && "No device mapped for address.");
}
void mem_write16(Memory *mem, uint16_t address, uint8_t value)
{
    for (size_t i = 0; i < mem->dev_count; i++)
    {
        Device dev = mem->devices[i];
        if (address >= dev.begin_address && address <= dev.end_address)
        {
            if (!dev.readonly)
            {
                dev.data[address - dev.begin_address] = u16_lo(value);
                dev.data[address - dev.begin_address + 1] = u16_hi(value);
            }
            return;
        }
    }

    assert(0 && "No device mapped for address.");
}
