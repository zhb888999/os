#include <mm/manager.h>
#include <multiboot/multiboot2.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/init.h>
#include <arch/x86_64.h>

#include <dev/serial.h>
#include <dev/vga.h>


extern uint32_t multiboot_info;

extern SegmentDescriptor64 gdt64_table[512];
extern InterruptDescriptor128 idt_table[256];


TSS64 tss;
MMFreePageManager mm_page_manager = {0};
MemorySegment mm_kerenl_segment;
uint64_t kernel_base_page_head;

static void init_pages(MemorySegmentPage *segments, int segment_count) {
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

static void init_gdt(void) {
    for (int i=0; i < 8; i++) printsf("GDT[%d] 0x%x\n", i, gdt64_table[i].attributes);
    set_segment_code(gdt64_table + 1, SEGMENT_PRESENT);
    set_segment_data(gdt64_table + 2, SEGMENT_PRESENT | SEGMENT_DATA_TYPE_W);
    set_segment_code(gdt64_table + 3, SEGMENT_PRESENT | SEGMENT_DPL_3 );
    set_segment_data(gdt64_table + 4, SEGMENT_PRESENT | SEGMENT_DATA_TYPE_W | SEGMENT_DPL_3);
    set_segment_tss((SegmentTSSDescriptor128 *)(gdt64_table + 5), (uint64_t)&tss, sizeof(tss), SEGMENT_PRESENT);
    // load_TR(5 * 1);
    for (int i=0; i < 8; i++) printsf("GDT[%d] 0x%x\n", i, gdt64_table[i].attributes);
}

void initialize() {
    MemorySegment segments[MAX_FREE_SEGMENT_COUNT];
    int segment_count = parse_multiboot2(multiboot_info, segments, &mm_kerenl_segment);
    init_pages(segments, segment_count);
    kernel_base_page_head = mm_alloc_pages(&mm_page_manager, ALLOC_PAGE_SIZE);
    init_gdt();
}