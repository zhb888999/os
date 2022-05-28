#include <kernel/interrupt.h>
#include <dev/mouse.h>
#include <apic/apic.h>
#include <arch/x86_64.h>
#include <dev/vga.h>
#include <asm/io.h>


static  MouseInput m_input = {0};

void mouse_handler(uint64_t nr, uint64_t parameter, IRQRegs *regs) {
    int8_t x = inb(PORT_MOUSE_DATA);
    printf("[%x]",x);
    if(m_input.phead == m_input.buf + MOUSE_BUF_SIZE)
        m_input.phead = m_input.buf;
    *m_input.phead = x;
    m_input.count++;
    m_input.phead++;
};

void mouse_exit(void) {
    unregister_irq(MOUSE_IRQ_NR);
};

static HWController mouse_contorller = {
    .enable = irq_enable,
    .disable = irq_disable,
    .install = irq_install,
    .uninstall = irq_uninstall,
    .ack = irq_edge_ack,
};

void mouse_init(void) {
    IRQEntry entry;
    m_input.phead = m_input.buf;
    m_input.ptail = m_input.buf;
    m_input.count = 0;
    entry.vector = irq2vector(MOUSE_IRQ_NR);
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

    wait_mouse_write();
	outb(KBCMD_EN_MOUSE_INTFACE, PORT_MOUSE_CMD);
    for(int i = 0; i < 1000*1000; i++) nop();
	wait_mouse_write();
	outb(KBCMD_SENDTO_MOUSE, PORT_MOUSE_CMD);
	wait_mouse_write();
	outb(MOUSE_ENABLE, PORT_MOUSE_DATA);
	wait_mouse_write();

    for(int i = 0; i < 1000*1000; i++) nop();
	wait_mouse_write();
	outb(MOUSE_CMD_WRITE_CMD, PORT_MOUSE_CMD);
	wait_mouse_write();
	outb(MOUSE_INIT_MODE, PORT_MOUSE_DATA);
    register_irq(MOUSE_IRQ_NR, &entry, &mouse_handler, 
        (uint64_t)&m_input, &mouse_contorller, "ps/2 mouse");

}