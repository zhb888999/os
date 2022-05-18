#include <dev/serial.h>
#include <dev/vga.h>
#include <kernel/init.h>
#include <kernel/global.h>
#include <arch/x86_64.h>


void kernel_main(void) {
    init_serial();
    init_vga(0xb8000, 80, 25);
    initialize();
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, 0, &eax, &ebx, &ecx, &edx);
    if((1<<9) & edx) printf("support APIC&xAPIC\n");
    if((1<<21) & ecx) printf("support x2APIC\n");
    for(;;) hlt();
}