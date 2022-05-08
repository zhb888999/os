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
} __attribute__((packed)) InterruptDescriptor64 ;

typedef struct {
    uint16_t length_1;
    uint16_t base_addr_1;
    uint8_t base_addr_2;
    uint8_t attributes;

} __attribute__((packed)) CodeDataSegmentDescriptor64;

#endif // ARCH_X86_64_H
