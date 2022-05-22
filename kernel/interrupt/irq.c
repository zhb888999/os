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
    // outb(0x20, 0x20);
    __asm__ __volatile__(
        "movq $0x00, %%rdx      \n\t"
        "movq $0x00, %%rax      \n\t"
        "movq $0x80b, %%rcx     \n\t"
        "wrmsr                  \n\t"
        :::"memory"
    );
}

void _irq44(void) {
    printf("mouse\n");
}