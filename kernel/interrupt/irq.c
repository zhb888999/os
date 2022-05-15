#include <dev/vga.h>
#include<asm/io.h>
#include<arch/x86_64.h>


void _irq32(void) {
    printf(".");
    outb(0x60, PIC_MASTER_OCW2);
}

void _irq33(void) {
    uint8_t keycode = inb(0x60);
    printf("%c", keycode);
    outb(0x20, 0x20);
}

void _irq44(void) {
    printf("mouse\n");
}