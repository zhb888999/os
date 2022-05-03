void kernel_main(unsigned int multiboot_information_address) {
    unsigned long long *addr = (unsigned long long *)0xb8040;
    *addr = 0x2f592f412f4b2f4a;
    *(addr + 4) = multiboot_information_address;
    for(;;);
}