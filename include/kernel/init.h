#ifndef KERNEL_INIT_H
#define KERNEL_INIT_H

#include <int.h>

#define P4 0
#define P3 1
#define P2 2
#define GDT 3
#define IDT 4
#define COMMON 5

#define COMM_GDT_PTR 0
#define COMM_IDT_PTR 10

#define ALLOC_PAGE_SIZE = 6

typedef struct {
    uint64_t min;
    uint64_t max;
} KernelMemMap;


typedef struct {
    uint64_t start;
    uint64_t end;
} MemorySegment;

typedef MemorySegment MemorySegmentPage;
#define MAX_FREE_SEGMENT_COUNT 10

#endif //KERNEL_BASE_H