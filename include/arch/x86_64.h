#ifndef ARCH_X86_64_H
#define ARCH_X86_64_H
#include <int.h>




#define SEGMENT_DPL_0 0x00
#define SEGMENT_DPL_1 0x20
#define SEGMENT_DPL_2 0x40
#define SEGMENT_DPL_3 0x60
#define SEGMENT_PRESENT 0x80
#define SEGMENT_AVL 0x1000

#define SEGMENT_DATA_TYPE_A 0x1
#define SEGMENT_DATA_TYPE_W 0x2
#define SEGMENT_DATA_TYPE_E 0x4

#define SEGMENT_CODE_TYPE_A 0x1
#define SEGMENT_CODE_TYPE_R 0x2
#define SEGMENT_CODE_TYPE_C 0x4

typedef struct {
    uint32_t ignore0;
    uint8_t ignore1;
    uint16_t attributes;
    uint8_t ignore2;
} __attribute__((packed)) SegmentDescriptor64;

#define set_code_segment_attr(value) (0x2018 | (value))
#define set_data_segment_attr(value) (0x0010 | (value))

#define SEGMENT_TSS_B 0x2

typedef struct {
    uint16_t length_1;
    uint16_t baseaddr_1;
    uint8_t baseaddr_2;
    uint16_t attributes_length_2;
    uint8_t baseaddr_3;
    uint32_t baseaddr_4;
    uint32_t zero;
} __attribute__((packed)) SegmentTSSDescriptor128;

#define set_tss_segment_attr(value) (0x09 | (value))

typedef struct {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero_must;
    uint8_t attributes;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t zero;
} __attribute__((packed)) SegmentCGDDescriptor128;

#define set_cgd_segment_attr(value) (0x0c | (value))


void set_segment_code(SegmentDescriptor64 *des, uint16_t attr);
void set_segment_data(SegmentDescriptor64 *des, uint16_t attr);
void set_segment_tss(SegmentTSSDescriptor128 *des, uint64_t baseaddr, uint32_t length, uint16_t attr);
void set_segment_cgd(SegmentCGDDescriptor128 *des, uint64_t offset, uint16_t selector, uint8_t attr);

/* TSS setting */
typedef struct {
	uint32_t  reserved0;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t reserved1;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t reserved2;
	uint16_t reserved3;
	uint16_t iomapbaseaddr;
}__attribute__((packed)) TSS64;

#define set_tr_register(n) __asm__ ("ltr %%ax"::"a"(n))

/* Interrupt setting */
#define GATE_INTERRUPT 0x8E
#define GATE_TRAP 0x8F
#define GATE_SYSTEM 0xEF
#define IDT_SELECTOR 0x08

#define enable_interrupt() __asm__ ("sti"::)
#define disable_interrupt() __asm__ ("cti"::)

typedef struct {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
   uint32_t offset_3;        // offset bits 32..63
   uint32_t zero;            // reserved
} __attribute__((packed)) InterruptDescriptor128;

void set_idt(InterruptDescriptor128 *des, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attributes);
void set_trap_gate(InterruptDescriptor128 *idt_table, uint32_t index, uint8_t ist, uint64_t addr);
void set_intr_gate(InterruptDescriptor128 *idt_table, uint32_t index, uint8_t ist, uint64_t addr);
void set_system_gate(InterruptDescriptor128 *idt_table, uint32_t index, uint8_t ist, uint64_t addr);

/* Page setting */
#define page_id(addr) ((addr) >> 12)
#define virtaddr_l4(addr) ((addr) >> 39 >> 0x1ff)
#define virtaddr_l3(addr) ((addr) >> 30 >> 0x1ff)
#define virtaddr_l2(addr) ((addr) >> 21 >> 0x1ff)
#define virtaddr_l1(addr) ((addr) >> 12 & 0x1ff )
#define virtaddr_offset(addr) ((addr) & 0xfff)

#define PAGE_PRESENT 0x1
#define PAGE_WRITABLE 0x2
#define PAGE_USER_ACCESSIBLE 0x4
#define PAGE_WRITE_THROUGH_CACHING 0x8
#define PAGE_DISABLE_CACHE 0x10
#define PAGE_ACCESSED 0x20
#define PAGE_DIRTY 0x40
#define PAGE_HUGE 0x80
#define PAGE_GLOBAL 0x100
#define PAGE_NO_EXECUTE 0x8000000000000000

#define page_entry(phyaddr, flags) ((phyaddr) & 0x1ffffffffff000 | (flags))

/* Other */

#define hlt() __asm__("hlt"::)
#endif // ARCH_X86_64_H
