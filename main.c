#include <int.h>
#include <dev/serial.h>
#include <dev/vga.h>

extern uint32_t multiboot_info;

void kernel_main(void) {
    init_serial();
    init_vga(0xb8000, 80, 25);
    printf("multiboot info: 0x%x", multiboot_info);
    printsf("multiboot info: 0x%x", multiboot_info);
    for(;;);
}