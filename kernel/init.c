#include <mm/manager.h>
#include <multiboot/multiboot2.h>
#include <kernel/interrupt/exceptions.h>
#include <kernel/interrupt/irq.h>
#include <kernel/init.h>
#include <arch/x86_64.h>
#include <asm/io.h>

#include <dev/serial.h>
#include <dev/vga.h>


extern uint32_t multiboot_info;

extern SegmentDescriptor64 gdt64_table[512];
extern uint64_t gdt_ptr;
extern InterruptDescriptor128 idt_table[256];
extern uint64_t idt_ptr;


TSS64 tss;
MMFreePageManager mm_page_manager = {0};
MemorySegment mm_kerenl_segment;
uint64_t kernel_base_page_head;

static void init_pages(MemorySegmentPage *segments, int segment_count);
static void init_gdt(void);
static void init_interrupt(void);

static void debug_print_idt(InterruptDescriptor128 *idt) {
    printsf("idt.offset_1: 0x%x\n", idt->offset_1);
    printsf("idt.ist: 0x%x\n", idt->ist);
    printsf("idt.type: 0x%x\n", idt->type_attributes);
    printsf("idt.offset_2: 0x%x\n", idt->offset_2);
    printsf("idt.offset_3: 0x%x\n", idt->offset_3);
    printsf("idt.selector: x0%x\n", idt->selector);
    printsf("idt.zero: 0x%x\n", idt->zero);
}


void initialize() {
    MemorySegment segments[MAX_FREE_SEGMENT_COUNT];
    int segment_count = parse_multiboot2(multiboot_info, segments, &mm_kerenl_segment);
    init_pages(segments, segment_count);
    kernel_base_page_head = mm_alloc_pages(&mm_page_manager, ALLOC_PAGE_SIZE);
    init_gdt();
    init_interrupt();
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
    set_tr_register(5 * 8);
}

static void init_interrupt(void) {
    for(uint32_t i=0; i < 256; i++) 
        set_intr_gate(idt_table, i, 0, (uint64_t) default_interrupt);
    // debug_print_idt(idt_table + 8);

    set_trap_gate(idt_table, 0, 0, (uint64_t) divide_error);
    set_trap_gate(idt_table, 1, 0, (uint64_t) debug);
    set_intr_gate(idt_table, 2, 0, (uint64_t) nmi);
    set_system_gate(idt_table, 3, 0, (uint64_t) int3);
    set_system_gate(idt_table, 4, 0, (uint64_t) overflow);
    set_system_gate(idt_table, 5, 0, (uint64_t) bounds);
    set_trap_gate(idt_table, 6, 0, (uint64_t) undefined_opcode);
    set_trap_gate(idt_table, 7, 0, (uint64_t) dev_not_available);
    set_trap_gate(idt_table, 8, 0, (uint64_t) double_fault);
    set_trap_gate(idt_table, 9, 0, (uint64_t) coprocessor_segment_overrun);
    set_trap_gate(idt_table, 10, 0, (uint64_t) invalid_tss);
    set_trap_gate(idt_table, 11, 0, (uint64_t) segment_not_present);
    set_trap_gate(idt_table, 12, 0, (uint64_t) stack_segment_fault);
    set_trap_gate(idt_table, 13, 0, (uint64_t) general_protection);
    set_trap_gate(idt_table, 14, 0, (uint64_t) page_fault);
    //15 reserved, can't use
    set_trap_gate(idt_table, 16, 0, (uint64_t) x87_fpu_error);
    set_trap_gate(idt_table, 17, 0, (uint64_t) alignment_check);
    set_trap_gate(idt_table, 18, 0, (uint64_t) machine_check);
    set_trap_gate(idt_table, 19, 0, (uint64_t) simd_exception);
    set_trap_gate(idt_table, 20, 0, (uint64_t) virtualization_exception);

    set_intr_gate(idt_table, 33, 0, (uint64_t) irq33);
    set_intr_gate(idt_table, 44, 0, (uint64_t) irq44);
    debug_print_idt(idt_table + 8);

    setup_pic(0x20);
    pci_mask_master(0xf9);
    pci_mask_slave(0xff);

    enable_interrupt();
}