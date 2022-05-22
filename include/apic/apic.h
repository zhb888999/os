#ifndef APIC_APIC_H
#define APIC_APIC_H

#include <int.h>

void init_localapic(void);
void init_ioapic(void);
uint32_t read_ioapic_rte32(uint8_t index);
uint64_t read_ioapic_rte64(uint8_t index);
void write_ioapic_rte32(uint8_t index, uint32_t value);
void write_ioapic_rte64(uint8_t index, uint64_t value);

#endif