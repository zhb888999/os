#include "kernel/interrupt/interrupt.h"
#include <dev/keyboard.h>
#include <apic/apic.h>
#include <arch/x86_64.h>
#include <dev/vga.h>

KeyBoardInput kb_input = {0};

void keyboard_handler(uint64_t nr, uint64_t parameter, IRQRegs *regs) {
    uint8_t x = inb(PORT_KB_DATA);
    printf("0x%x", x);
    if(kb_input.phead == kb_input.buf + KB_BUF_SIZE)
        kb_input.phead = kb_input.buf;
    *kb_input.phead = x;
    kb_input.count++;
    kb_input.phead++;
};

void keyboard_exit(void) {
    unregister_irq(KB_IRQ_NR);
};

HWController keyboard_contorller = {
    .enable = irq_enable,
    .disable = irq_disable,
    .install = irq_install,
    .uninstall = irq_uninstall,
    .ack = irq_edge_ack,
};

void keyboard_init(void) {
    IRQEntry entry;
    kb_input.phead = kb_input.buf;
    kb_input.ptail = kb_input.buf;
    kb_input.count = 0;
    entry.vector = irq2vector(KB_IRQ_NR);
    entry.deliver_mode = APIC_ICR_IOAPIC_Fixed;
    entry.dest_mode = ICR_IOAPIC_DELV_PHYSICAL;
    entry.deliver_status = APIC_ICR_IOAPIC_Idle;
    entry.polarity = APIC_IOAPIC_POLARITY_HIGH;
    entry.irr = APIC_IOAPIC_IRR_RESET;
    entry.trigger = APIC_ICR_IOAPIC_Edge;
    entry.mask = APIC_ICR_IOAPIC_Masked;
    entry.reserved = 0;

    entry.destination.physical.reserved1 = 0;
    entry.destination.physical.phy_dest = 0;
    entry.destination.physical.reserved2 = 0;

    wait_keyboard_write();
    outb(KBCMD_WRITE_CMD, PORT_KB_CMD);
    wait_keyboard_write();
    outb(KB_INIT_MODE, PORT_KB_DATA);
    for(int i = 0; i < 1000*1000; i++) nop();

    register_irq(KB_IRQ_NR, &entry,  &keyboard_handler, 
        (uint64_t)&kb_input, &keyboard_contorller, "ps/2 keyboard");
}