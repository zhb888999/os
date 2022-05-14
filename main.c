#include <dev/serial.h>
#include <dev/vga.h>
#include <kernel/init.h>
#include <kernel/global.h>
#include <arch/x86_64.h>


void kernel_main(void) {
    init_serial();
    init_vga(0xb8000, 80, 25);
    initialize();
    int b = 0;
    int c = 0;
    // int a = b/c;
    // printf("0 -> %d\n", a);
    for(;;) hlt();
}