#include <int.h>
#include <dev/serial.h>
#include <dev/vga.h>
#include <multiboot/multiboot2.h>
#include <mm/manager.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/base.h>

extern uint32_t multiboot_info;



void kernel_main(void) {
    init_serial();
    init_vga(0xb8000, 80, 25);
    init_mm_page_manager(&mm_page_manager);
    printf("multiboot info: 0x%x\n", multiboot_info);
    parse_multiboot2(multiboot_info, &mm_page_manager);
    mm_info(&mm_page_manager);
    init_interrupt();
    enable_interrupt();
    for(;;) __asm__("hlt");
}