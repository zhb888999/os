#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <int.h>
#include <arch/x86_64.h>

#define GATE_INTERRUPT              0x8E
#define GATE_TRAP                   0x8F
#define GATE_SYSTEM                 0xEF
#define GATE_SYSTEM_INTERRUPT       0xEF

#define enable_interrupt() __asm__ ("sti"::)

#define disable_interrupt() __asm__ ("cti"::)


void init_interrupt(void);
void set_interrupt(uint32_t index, uint8_t gate_type, uint8_t ist, void *addr);

void default_interrupt(void);
#endif // INTERRUPT_H