#ifndef MM_KMANAGE_H
#define MM_KMANAGE_H

#include <stdint.h>

#define BUMP_ALLOCATER_BITS 25
#define BUMP_ALLOCATER_SIZE 1 << BUMP_ALLOCATER_BITS
#define BUMP_ALLOCATER_NR 1 << (32 - BUMP_ALLOCATER_BITS)

typedef struct {
    uint64_t start;
    uint64_t end;
    uint64_t next;
    uint64_t allocations;
    uint64_t continuous;
} BumpAllocator;

#define ALIGN_UP(addr, align) (((addr) + (align) - 1) & !((align) - 1))

#endif