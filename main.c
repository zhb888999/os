#include <dev/serial.h>
#include <dev/vga.h>
#include <kernel/init.h>
#include <kernel/global.h>
#include <arch/x86_64.h>
#include <dev/keyboard.h>
#include <kernel/time.h>


void kernel_main(void) {
    init_serial();
    init_vga(0xb8000, 80, 25);
    initialize();
    // Time t;
    // int second = 0;
    // for(;;) {
    //     cmos_time(&t);
    //     if(t.second != second) {
    //         printf("%x-%x-%x %x:%x:%x\n", t.year, t.month, t.day, t.hour, t.minute, t.second);
    //         second = t.second;
    //     }
    // }
    for(;;) {hlt(); analysis_keycode();}
}