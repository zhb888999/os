#include <mm/pmanager.h>
#include <multiboot/multiboot2.h>
#include <kernel/interrupt.h>
#include <kernel/init.h>
#include <arch/x86_64.h>
#include <asm/io.h>

#include <dev/serial.h>
#include <dev/vga.h>
#include <dev/keyboard.h>
#include <dev/mouse.h>
#include <dev/disk.h>
#include <apic/apic.h>

extern uint32_t multiboot_info;

extern SegmentDescriptor64 gdt64_table[512];
extern uint64_t gdt_ptr;
extern InterruptDescriptor128 idt_table[256];
extern uint64_t idt_ptr;
extern uint64_t p4_table[512];
extern uint64_t p3_table[512];

TSS64 tss;
MemorySegment kerenl_segment;

static void init_pages(MemorySegmentPage *segments, int segment_count);
static void init_gdt(void);
static void init_apic(void);
static void init_8259a(void);

void initialize() {
    MemorySegment segments[MAX_FREE_SEGMENT_COUNT];
    int segment_count = parse_multiboot2(multiboot_info, segments, &kerenl_segment);
    init_pages(segments, segment_count);
    init_gdt();
    init_idt();
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);
    if((1<<9) & edx) printf("support APIC&xAPIC\n");
    if((1<<21) & ecx) printf("support x2APIC\n");
    init_apic();
}

static void init_pages(MemorySegmentPage *segments, int segment_count) {
    uint64_t kernel_min_page = page_id(kerenl_segment.start);
    uint64_t kernel_max_page = page_id(kerenl_segment.end);
    for(int i = 0; i < segment_count; i++, segments++) {
        if(segments->start < 3) segments->start = 3;
        if(segments->end - segments->start < 1) continue;

        int cross = 0;
        if(segments->start <= kernel_min_page && segments->end >= kernel_min_page) {
            pfree(segments->start, kernel_min_page - segments->start);
            cross = 1;
        }
        if(segments->start <= kernel_max_page && segments->end >= kernel_max_page) {
            pfree(kernel_max_page + 1, segments->end - kernel_max_page - 1);
            cross = 1;
        }
        if (cross) continue;
            
        pfree(segments->start, segments->end - segments->start);
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


static void init_8259a(void) {
    setup_pic(0x20);
    setup_pit();
    pci_mask_master(0xfc);
    pci_mask_slave(0xff);
}

static void init_apic(void) {
    pci_mask_master(0xff);
    pci_mask_slave(0xff);
    enable_imcr();
    init_localapic();
    init_ioapic();
    enable_ioapic();
    sti();
    keyboard_init();
    // mouse_init();
    disk_init();
}