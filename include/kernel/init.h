#ifndef KERNEL_INIT_H
#define KERNEL_INIT_H

#include <int.h>

#define P4_HEAD 0
#define P4_SIZE 1

#define P3_HEAD 1
#define P3_SIZE 1

#define P2_HEAD 2
#define P2_SIZE 16

#define GDT_HEAD 18
#define IDT_HEAD 19

#define COMMON_HEAD 20
#define COMMON_SIZE 1

#define COMM_GDT_PTR 0
#define COMM_IDT_PTR 10

#define ALLOC_PAGE_SIZE 20

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

void initialize(void);

#endif //KERNEL_BASE_H