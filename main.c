#include <dev/serial.h>
#include <dev/vga.h>
#include <kernel/init.h>
#include <kernel/global.h>
#include <arch/x86_64.h>
#include <dev/keyboard.h>


void kernel_main(void) {
    init_serial();
    init_vga(0xb8000, 80, 25);
    initialize();
    for(;;) {hlt(); analysis_keycode();}
}