#include <kernel/interrupt/interrupt.h>
#include <apic/apic.h>
#include <arch/x86_64.h>
#include <dev/vga.h>
#include <dev/disk.h>
#include <asm/io.h>


void disk_handler(uint64_t nr, uint64_t parameter, IRQRegs *regs) {
	printf(">>>>>>>>>>");
	char data[300] = {0};
	insw(&data, 256, PORT_DISK1_DATA);
	data[256] = '\0';
	printf("%s", data);
};

void disk_exit(void) {
    unregister_irq(DISK_IRQ_NR);
};

static HWController disk_contorller = {
    .enable = irq_enable,
    .disable = irq_disable,
    .install = irq_install,
    .uninstall = irq_uninstall,
    .ack = irq_edge_ack,
};

void disk_init(void) {
    IRQEntry entry;
    entry.vector = irq2vector(DISK_IRQ_NR);
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

    register_irq(DISK_IRQ_NR, &entry,  &disk_handler, 
        0, &disk_contorller, "disk0");

	outb(0, PORT_DISK1_ALT_STA_CTL);
	outb(0, PORT_DISK1_SECTOR_CNT);
	outb(0, PORT_DISK1_SECTOR_LOW);
	outb(0, PORT_DISK1_SECTOR_MID);
	outb(0, PORT_DISK1_SECTOR_HIGH);
	outb(0xe0, PORT_DISK1_DEVICE);
	outb(0xec, PORT_DISK1_STATUS_CMD);
}
