#include <dev/vga.h>
void _irq32(void) {
    printf("timer\n");
}

void _irq33(void) {
    printf("keyboard\n");
}

void _irq44(void) {
    printf("mouse\n");
}