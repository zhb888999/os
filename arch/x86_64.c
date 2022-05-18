#include <arch/x86_64.h>
#include <asm/io.h>

inline void set_segment_code(SegmentDescriptor64 *des, uint16_t attr) {
    des->ignore0 = 0;
    des->ignore1 = 0;
    des->attributes = set_code_segment_attr(attr);
    des->ignore2 = 0;
}

inline void set_segment_data(SegmentDescriptor64 *des, uint16_t attr) {
    des->ignore0 = 0;
    des->ignore1 = 0;
    des->attributes = set_data_segment_attr(attr);
    des->ignore2 = 0;
}

inline void set_segment_tss(SegmentTSSDescriptor128 *des, uint64_t baseaddr, uint32_t length, uint16_t attr) {
    des->length_1 = length & 0xffff;
    des->baseaddr_1 = baseaddr & 0xffff;
    des->baseaddr_2 = (baseaddr & 0xff0000) >> 16;
    des->attributes_length_2 = ((length & 0xf0000) >> 8) | set_tss_segment_attr(attr);
    des->baseaddr_3 = (baseaddr & 0xff000000) >> 24;
    des->baseaddr_4 = baseaddr >> 32;
    des->zero = 0;
}

inline void set_segment_cgd(SegmentCGDDescriptor128 *des, uint64_t offset, uint16_t selector, uint8_t attr) {
    des->offset_1 = offset << 48 >> 48;
    des->selector = selector;
    des->zero_must = 0;
    des->attributes = set_cgd_segment_attr(attr); 
    des->offset_2 = offset << 32 >> 48;
    des->offset_3 = offset >> 32;
    des->zero = 0;
}

inline void set_tss64(TSS64 *tss, uint64_t rsp0, uint64_t rsp1, uint64_t rsp2, 
    uint64_t ist1, uint64_t ist2, uint64_t ist3, uint64_t ist4, uint64_t ist5, uint64_t ist6, uint64_t ist7) {
        tss->rsp0 = rsp0;
        tss->rsp1 = rsp1;
        tss->rsp2 = rsp2;
        tss->ist1 = ist1;
        tss->ist2 = ist2;
        tss->ist3 = ist3;
        tss->ist4 = ist4;
        tss->ist5 = ist5;
        tss->ist6 = ist6;
        tss->ist7 = ist7;
        tss->iomapbaseaddr = 0;
        tss->reserved0 = 0;
        tss->reserved1 = 0;
        tss->reserved2 = 0;
        tss->reserved3 = 0;
    }

inline void set_idt(InterruptDescriptor128 *des, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attributes) {
    des->offset_1 = offset & 0xffff;
    des->ist = ist;
    des->selector = selector;
    des->zero = 0;
    des->type_attributes = set_cgd_segment_attr(type_attributes); 
    des->offset_2 = offset  >> 16 & 0xffff;
    des->offset_3 = offset >> 32;
    des->zero = 0;
}

inline void set_trap_gate(InterruptDescriptor128 *idt_table, uint32_t index, uint8_t ist, uint64_t addr) {
    set_idt(idt_table + index, addr, IDT_SELECTOR, ist, GATE_TRAP);
}

inline void set_intr_gate(InterruptDescriptor128 *idt_table, uint32_t index, uint8_t ist, uint64_t addr) {
    set_idt(idt_table + index, addr, IDT_SELECTOR, ist, GATE_INTERRUPT);
}

inline void set_system_gate(InterruptDescriptor128 *idt_table, uint32_t index, uint8_t ist, uint64_t addr) {
    set_idt(idt_table + index, addr, IDT_SELECTOR, ist, GATE_SYSTEM);
}

inline void setup_pic(uint8_t start_id) {
    outb(0x11, PIC_MASTER_ICW1);
    outb(start_id, PIC_MASTER_ICW2);
    outb(0x04, PIC_MASTER_ICW3);
    outb(0x01, PIC_MASTER_ICW4);

    outb(0x11, PIC_SLAVE_ICW1);
    outb(start_id + 8, PIC_SLAVE_ICW2);
    outb(0x02, PIC_SLAVE_ICW3);
    outb(0x01, PIC_SLAVE_ICW4);
}

inline void pci_mask_master(uint8_t mask) {
    outb(mask, PIC_MASTER_OCW1);
}

inline void pci_mask_slave(uint8_t mask) {
    outb(mask, PIC_SLAVE_OCW1);
}

inline void setup_pit() {
    outb(0x34, PIT_CTRL);
    // outb(0x9c, PIT_CNT0);
    // outb(0x2e, PIT_CNT0);
    outb(0xff, PIT_CNT0);
    outb(0xff, PIT_CNT0);
}

inline void cpuid(uint32_t mop, uint32_t sop, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    __asm__ __volatile__(
        "cpuid  \n\t"
        :"=a"(*a),"=b"(*b),"=c"(*c),"=d"(*d)
        :"0"(mop),"2"(sop)
    );
}