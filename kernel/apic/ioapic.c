#include <apic/apic.h>
#include <arch/x86_64.h>
#include <int.h>

static uint8_t *IOREGSEL = (uint8_t *)IO_APIC_REG_BASE;
static uint32_t *IOWIN = (uint32_t *)(IO_APIC_REG_BASE + 0x10);
static uint32_t *EOI = (uint32_t *)(IO_APIC_REG_BASE + 0x40);

uint32_t read_ioapic_rte32(uint8_t index) {
    uint32_t value;
    *IOREGSEL = index + 1;
    mfence();
    value = *IOWIN;
    mfence();
    return value;
}

uint64_t read_ioapic_rte64(uint8_t index) {
    uint64_t value;
    *IOREGSEL = index + 1;
    mfence();
    value = *IOWIN;
    value <<= 32;
    mfence();
    *IOREGSEL = index;
    mfence();
    value |= *IOWIN;
    mfence();
    return value;
}

void write_ioapic_rte32(uint8_t index, uint32_t value) {
    *IOREGSEL = index;
    mfence();
    *IOWIN = value;
    mfence();
}

void write_ioapic_rte64(uint8_t index, uint64_t value) {
    *IOREGSEL = index;
    mfence();
    *IOWIN = value & 0xffffffff;
    value >>= 32;
    mfence();
    *IOREGSEL = index + 1;
    mfence();
    *IOWIN = value & 0xffffffff;
    mfence();
}

void init_ioapic(void) {
    write_ioapic_rte32(0x00, 0x0f000000);
    for(uint8_t i = 0x10; i < 0x40; i += 2)
        write_ioapic_rte64(i, 0x10020 + ((i - 0x10) >> 1));
    write_ioapic_rte64(0x12, 0x21);
}