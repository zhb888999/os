#include <arch/x86_64.h>

#include "exceptions.h"
#include "irq.h"
extern InterruptDescriptor128 idt_table[256];

void init_idt(void) {
        for(uint32_t i=0; i < 256; i++) 
        set_intr_gate(idt_table, i, 0, (uint64_t) default_interrupt);

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

    set_intr_gate(idt_table, 32, 0, (uint64_t) irq00);
    set_intr_gate(idt_table, 33, 0, (uint64_t) irq01);
    set_intr_gate(idt_table, 34, 0, (uint64_t) irq02);
    set_intr_gate(idt_table, 35, 0, (uint64_t) irq03);
    set_intr_gate(idt_table, 36, 0, (uint64_t) irq04);
    set_intr_gate(idt_table, 37, 0, (uint64_t) irq05);
    set_intr_gate(idt_table, 38, 0, (uint64_t) irq06);
    set_intr_gate(idt_table, 39, 0, (uint64_t) irq07);
    set_intr_gate(idt_table, 40, 0, (uint64_t) irq08);
    set_intr_gate(idt_table, 41, 0, (uint64_t) irq09);
    set_intr_gate(idt_table, 42, 0, (uint64_t) irq0a);
    set_intr_gate(idt_table, 43, 0, (uint64_t) irq0b);
    set_intr_gate(idt_table, 44, 0, (uint64_t) irq0c);
    set_intr_gate(idt_table, 45, 0, (uint64_t) irq0d);
    set_intr_gate(idt_table, 46, 0, (uint64_t) irq0e);
    set_intr_gate(idt_table, 47, 0, (uint64_t) irq0f);
    set_intr_gate(idt_table, 48, 0, (uint64_t) irq10);
    set_intr_gate(idt_table, 49, 0, (uint64_t) irq11);
    set_intr_gate(idt_table, 50, 0, (uint64_t) irq12);
    set_intr_gate(idt_table, 51, 0, (uint64_t) irq13);
    set_intr_gate(idt_table, 52, 0, (uint64_t) irq14);
    set_intr_gate(idt_table, 53, 0, (uint64_t) irq15);
    set_intr_gate(idt_table, 54, 0, (uint64_t) irq16);
    set_intr_gate(idt_table, 55, 0, (uint64_t) irq17);
}