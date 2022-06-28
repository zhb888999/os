#include <kernel/interrupt.h>
#include <apic/apic.h>
#include <arch/x86_64.h>
#include <dev/vga.h>
#include <dev/disk.h>
#include <asm/io.h>


void disk_handler(uint64_t nr, uint64_t parameter, IRQRegs *regs) {
	// char data[300] = {0};
	// insb(&data, 256, PORT_DISK0_DATA);
	// data[256] = '\0';
	// for(int i=0; i<300; i++) printf("[%c]", data[i]);
	// printf("[[%s]]\n", data);
    printf("Write One Sector Finished:%x\n", inb(PORT_DISK0_STATUS_CMD));
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
    return;
    outw(0, PORT_DISK0_ALT_STA_CTL);
    while(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_BUSY);
    outw(0x40, PORT_DISK0_DEVICE);

    // outw(0, PORT_DISK0_ERR_FEATURE);
    outw(0, PORT_DISK0_SECTOR_CNT);
    outw(0x0, PORT_DISK0_SECTOR_LOW);
    printf(">>>>");
    outw(0x0, PORT_DISK0_SECTOR_MID);
    outw(0x0, PORT_DISK0_SECTOR_HIGH);

    // outw(0, PORT_DISK0_ERR_FEATURE);
    outw(1, PORT_DISK0_SECTOR_CNT);
    outw(0x0, PORT_DISK0_SECTOR_LOW);
    outw(0x0, PORT_DISK0_SECTOR_MID);
    outw(0x4, PORT_DISK0_SECTOR_HIGH);

    while(!(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_READY));
    printf("Send CMD:0x%x\n", inb(PORT_DISK0_STATUS_CMD));

    outw(0x34, PORT_DISK0_STATUS_CMD);

    while(!(inb(PORT_DISK0_STATUS_CMD) & DISK_STATUS_REQ));
    char a[512];
    for(int i=0; i < 512; i++) a[i] = 0xa5;
    outsw(&a, PORT_DISK0_DATA, 512);
    printf("Send!\n");
}
