#ifndef KERNEL_GLOBAL_H
#define KERNEL_GLOBAL_H

#include <mm/manager.h>
#include <kernel/init.h>

extern MMFreePageManager mm_page_manager;
extern MemorySegment mm_kernel_segment;

#endif