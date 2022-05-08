#include "mm/manager.h"
#include <dev/serial.h>
#include <dev/vga.h>
#include <kernel/init.h>
#include <kernel/global.h>


void kernel_main(void) {
    init_serial();
    init_vga(0xb8000, 80, 25);
    initialize();
    mm_info(&mm_page_manager);
    uint64_t start = mm_alloc_pages(&mm_page_manager, 100);
    mm_info(&mm_page_manager);
    mm_free_pages(&mm_page_manager, start, 100);
    mm_info(&mm_page_manager);
    start = mm_alloc_pages(&mm_page_manager, 10000);
    mm_info(&mm_page_manager);
    mm_free_pages(&mm_page_manager, start, 10000);
    mm_info(&mm_page_manager);
    // init_interrupt();
    // enable_interrupt();
    for(;;) __asm__("hlt");
}