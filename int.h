#ifndef INCLUDE_6502_INT_H_
#define INCLUDE_6502_INT_H_

#include <stdbool.h>
#include <stdint.h>

uint8_t u16_lo(uint16_t u16);
uint8_t u16_hi(uint16_t u16);
uint16_t u16_from_lohi(uint8_t lo, uint8_t hi);
bool u8sign(uint8_t value);

#endif // INCLUDE_6502_INT_H_
