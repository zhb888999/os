#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <int.h>

#define GATE_INTERRUPT              0x8E
#define GATE_TRAP                   0x8F
#define GATE_SYSTEM                 0xEF
#define GATE_SYSTEM_INTERRUPT       0xEF

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
} InterruptDescriptor64;

void init_interrupt();
void set_interrupt(uint32_t index, uint8_t gate_type, uint8_t ist, void *addr);

void default_interrupt(void);
#endif // INTERRUPT_H