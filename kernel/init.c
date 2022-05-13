#include <mm/manager.h>
#include <multiboot/multiboot2.h>
#include <kernel/interrupt.h>
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

static void init_pages(MemorySegmentPage *segments, int segment_count);
static void init_gdt(void);
static void set_interrupt(uint32_t index, uint8_t type, uint8_t ist, uint64_t addr);
static void init_interrupt(void);


void initialize() {
    MemorySegment segments[MAX_FREE_SEGMENT_COUNT];
    int segment_count = parse_multiboot2(multiboot_info, segments, &mm_kerenl_segment);
    init_pages(segments, segment_count);
    kernel_base_page_head = mm_alloc_pages(&mm_page_manager, ALLOC_PAGE_SIZE);
    init_gdt();
    init_interrupt();
    enable_interrupt();
}

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
    set_segment_code(gdt64_table + 1, SEGMENT_PRESENT);
    set_segment_data(gdt64_table + 2, SEGMENT_PRESENT | SEGMENT_DATA_TYPE_W);
    set_segment_code(gdt64_table + 3, SEGMENT_PRESENT | SEGMENT_DPL_3 );
    set_segment_data(gdt64_table + 4, SEGMENT_PRESENT | SEGMENT_DATA_TYPE_W | SEGMENT_DPL_3);
    set_segment_tss((SegmentTSSDescriptor128 *)(gdt64_table + 5), (uint64_t)&tss, sizeof(tss), SEGMENT_PRESENT);
    load_TR(5);
}

inline static void set_interrupt(uint32_t index, uint8_t type, uint8_t ist, uint64_t addr) {
    idt_table[index].offset_1 = addr & 0xffff;
    idt_table[index].ist = ist;
    idt_table[index].type_attributes = type;
    idt_table[index].offset_2 = addr  >> 16 & 0xffff;
    idt_table[index].offset_3 = addr  >> 32;
    idt_table[index].selector = 0x08;
    idt_table[index].zero = 0;
}

static void init_interrupt(void) {
    printsf("interrp %X\n", (uint64_t) default_interrupt);
    for(uint32_t i=0; i < 256; i++) {
        set_interrupt(i, GATE_INTERRUPT, 0, (uint64_t) default_interrupt);
    }
    printsf("idt.offset_1: %x\n", idt_table[0].offset_1);
    printsf("idt.ist: %x\n", idt_table[0].ist);
    printsf("idt.type: %x\n", idt_table[0].type_attributes);
    printsf("idt.offset_2: %x\n", idt_table[0].offset_2);
    printsf("idt.offset_3: %x\n", idt_table[0].offset_3);
    printsf("idt.selector: %x\n", idt_table[0].selector);
    printsf("idt.zero: %x\n", idt_table[0].zero);
}