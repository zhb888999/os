#include <kernel/interrupt.h>
#include <int.h>
#include <kernel/time.h>
#include <arch/x86_64.h>
#include <apic/apic.h>
#include <dev/serial.h>

void cmos_time(Time *time) {
    cli();
    do {
        time->year = CMOS_READ(0x09) + CMOS_READ(0x32) * 0x100;
        time->month = CMOS_READ(0x08);
        time->day = CMOS_READ(0x07);
        time->hour = CMOS_READ(0x04);
        time->minute = CMOS_READ(0x02);
        time->second = CMOS_READ(0x00);
    }while(time->second != CMOS_READ(0x00));
    outb(0x00, 0x70);
    sti();
}

static HWController hpet_contorller = {
    .enable = irq_enable,
    .disable = irq_disable,
    .install = irq_install,
    .uninstall = irq_uninstall,
    .ack = irq_edge_ack,
};

void hpet_handler(uint64_t nr, uint64_t parameter, IRQRegs *regs) {
    printsf(".");
}

void hpet_init(void) {
    uint32_t x;
    uint32_t *p;
    uint8_t *hept_addr = (uint8_t *)(0xfed00000);

    outl(0x8000f8f0, 0xcf8);
    x = inl(0xcfc);
    x = x & 0xffffc000;

    if(x > 0xfec00000 && x < 0xfee00000)
        p = (uint32_t *)(x + 0x3404UL);

    *p = 0x80;
    mfence();

    IRQEntry entry;    
    entry.vector = irq2vector(HPET_IRQ_NR);
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

    register_irq(HPET_IRQ_NR, &entry, &hpet_handler, 0, &hpet_contorller, "HPET");
}