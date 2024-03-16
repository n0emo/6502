#include "int.h"

uint8_t u16_lo(uint16_t u16)
{
    return u16 & 0x00FF;
}

uint8_t u16_hi(uint16_t u16)
{
    return (u16 & 0xFF00) >> 8;
}

uint16_t u16_from_lohi(uint8_t lo, uint8_t hi)
{
    return lo | (hi << 8);
}

bool u8sign(uint8_t value)
{
    return value & 0x80;
}
