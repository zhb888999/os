#ifndef VGA_H
#define VGA_H

#include <int.h>

#define ATTRIBUTE 15

#define POS_PORT 0x3d4
#define VAL_PORT 0x3d5

void init_vga(uintptr_t addr, uint32_t xlimit, uint32_t ylimit);
void printf(const char *format, ...);

#endif // VGA_H