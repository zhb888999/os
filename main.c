#include <mm/manager.h>
#include <dev/serial.h>
#include <dev/vga.h>
#include <kernel/init.h>
#include <kernel/global.h>


void kernel_main(void) {
    init_serial();
    init_vga(0xb8000, 80, 25);
    initialize();
    // init_interrupt();
    // enable_interrupt();
    for(;;) __asm__("hlt");
}