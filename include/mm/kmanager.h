#ifndef MM_KMANAGE_H
#define MM_KMANAGE_H

#include "mm/pmanager.h"
#include <int.h>

#define BUMP_ALLOCATER_BITS 25
#define BUMP_ALLOCATER_SIZE (1 << BUMP_ALLOCATER_BITS)
#define BUMP_ALLOCATER_NR (1 << (32 - BUMP_ALLOCATER_BITS))
#define BUMP_INDEX(addr) ((addr >> BUMP_ALLOCATER_BITS))
typedef struct {
    uint64_t start;
    uint64_t end;
    uint64_t next;
    uint64_t allocations;
    uint64_t continuous;
} BumpAllocator;

#define ALIGN_UP(addr, align) (((addr) + (align) - 1) & !((align) - 1))
uint64_t kmalloc(uint64_t size, uint64_t align);
void kfree(uint64_t address);
void bumps_info(void);
void bumps_info2(int start, int end);

#endif