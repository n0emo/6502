#ifndef SRC_ROM_H_
#define SRC_ROM_H_

#include <stdint.h>

unsigned char *read_entire_file_binary(const char *path, uintptr_t *len);
uint8_t *load_rom(const char *path);

#endif // SRC_ROM_H_
