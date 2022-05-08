#include <mm/manager.h>
#include <multiboot/multiboot2.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/init.h>

extern uint32_t multiboot_info;
MMFreePageManager mm_page_manager;
MemorySegment mm_kerenl_segment;

void init() {
    MemorySegment segments[MAX_FREE_SEGMENT_COUNT];
    int segment_count = 0;
    init_mm_page_manager(&mm_page_manager);
    parse_multiboot2(multiboot_info, segments, &mm_kerenl_segment, &segment_count);

        // init mm_page
    for(int i = 0; i < segment_count; i++) {
        if(segments[i].start < 3) segments[i].start = 3;
        if(segments[i].end - segments[i].start < 1) continue;

        int cross = 0;
        if(segments[i].start <= mm_kernel_min && segments[i].end >= mm_kernel_min) {
            mm_free_pages(man, segments[i].start, mm_kernel_min - segments[i].start);
            cross = 1;
        }
        if(segments[i].start <= mm_kernel_max && segments[i].end >= mm_kernel_max) {
            mm_free_pages(man, mm_kernel_max + 1, segments[i].end - mm_kernel_max - 1);
            cross = 1;
        }
        if (cross) continue;
            
        mm_free_pages(man, segments[i].start, segments[i].end - segments[i].start);
    }
}