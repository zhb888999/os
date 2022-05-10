#include <mm/manager.h>
#include <multiboot/multiboot2.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/init.h>
#include <arch/x86_64.h>
#include <dev/serial.h>


extern uint32_t multiboot_info;
MMFreePageManager mm_page_manager;
MemorySegment mm_kerenl_segment;
uint64_t kernel_base_page_head;

static void free_pages(MemorySegmentPage *segments, int segment_count) {
    uint64_t kernel_min_page = page_id(mm_kerenl_segment.start);
    uint64_t kernel_max_page = page_id(mm_kerenl_segment.end);
    for(int i = 0; i < segment_count; i++, segments++) {
        if(segments->start < 3) segments->start = 3;
        if(segments->end - segments->start < 1) continue;

        int cross = 0;
        if(segments->start <= kernel_min_page && segments->end >= kernel_min_page) {
            mm_free_pages(&mm_page_manager, segments->start, kernel_min_page - segments->start);
            cross = 1;
        }
        if(segments->start <= kernel_max_page && segments->end >= kernel_max_page) {
            mm_free_pages(&mm_page_manager, kernel_max_page + 1, segments->end - kernel_max_page - 1);
            cross = 1;
        }
        if (cross) continue;
            
        mm_free_pages(&mm_page_manager, segments->start, segments->end - segments->start);
    }
}

void initialize() {
    MemorySegment segments[MAX_FREE_SEGMENT_COUNT];
    init_mm_page_manager(&mm_page_manager);
    int segment_count = parse_multiboot2(multiboot_info, segments, &mm_kerenl_segment);
    free_pages(segments, segment_count);
    kernel_base_page_head = mm_alloc_pages(&mm_page_manager, ALLOC_PAGE_SIZE);
}