#ifndef ARCH_X86_64_H
#define ARCH_X86_64_H
#include <int.h>

#define GATE_INTERRUPT              0x8E
#define GATE_TRAP                   0x8F
#define GATE_SYSTEM                 0xEF
#define GATE_SYSTEM_INTERRUPT       0xEF

typedef struct {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
   uint32_t offset_3;        // offset bits 32..63
   uint32_t zero;            // reserved
} __attribute__((packed)) InterruptDescriptor128;

#define SEGMENT_DPL_0 0x00
#define SEGMENT_DPL_1 0x20
#define SEGMENT_DPL_2 0x40
#define SEGMENT_DPL_4 0x60
#define SEGMENT_PRESENT 0x80
#define SEGMENT_ABL 0x1000

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

#define page_id(addr) ((addr) >> 12)

void set_segment_code(SegmentDescriptor64 *des, uint16_t attr);
void set_segment_data(SegmentDescriptor64 *des, uint16_t attr);
void set_segment_tss(SegmentTSSDescriptor128 *des, uint64_t baseaddr, uint32_t length, uint16_t attr);
void set_segment_cgd(SegmentCGDDescriptor128 *des, uint64_t offset, uint16_t selector, uint8_t attr);
void set_idt(InterruptDescriptor128 *des, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attributes);


#endif // ARCH_X86_64_H
