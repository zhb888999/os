#include <kernel/interrupt/interrupt.h>

extern void default_interrupt(void);
extern InterruptDescriptor64 idt_table[];

inline void set_interrupt(uint32_t index, uint8_t gate_type, uint8_t ist, void *addr) {
    idt_table[index].offset_1 = (uint64_t)addr & 0xffff;
    idt_table[index].ist = ist;
    idt_table[index].type_attributes = gate_type;
    idt_table[index].offset_2 = (uint64_t)addr  >> 16 & 0xffff;
    idt_table[index].offset_3 = (uint64_t)addr  >> 32;
    idt_table[index].selector = 0x08;
    idt_table[index].zero = 0;
}

void init_interrupt() {
    for(uint32_t i=0; i < 256; i++) {
        set_interrupt(i, GATE_INTERRUPT, 1, default_interrupt);
    }
}