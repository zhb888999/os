#include <dev/vga.h>
#include <dev/serial.h>
#include <arch/x86_64.h>
#include <asm/io.h>

void _default_interrupt(void) {
    printsf("unkonw interrupt!\n");
    printf("unkonw interrupt!\n");
    for(;;) hlt();
}

void _divide_error(uint64_t regs,uint64_t error_code)
{
    printf("divide -> %X\n", error_code);
    printsf("divide -> %X\n", error_code);
    for(;;) hlt();
}

void _debug(uint64_t regs,uint64_t error_code)
{
    printf("debug -> %X\n", error_code);
    printsf("debug -> %X\n", error_code);
    for(;;) hlt();
}

void _nmi(uint64_t regs,uint64_t error_code)
{
    printf("nmi -> %X\n", error_code);
    printsf("nmi -> %X\n", error_code);
    for(;;) hlt();
}


void _int3(uint64_t regs,uint64_t error_code)
{
    printf("int3 -> %X\n", error_code);
    printsf("int3 -> %X\n", error_code);
    for(;;) hlt();
}

void _overflow(uint64_t regs,uint64_t error_code)
{
    printf("overflow -> %X\n", error_code);
    printsf("overflow -> %X\n", error_code);
    for(;;) hlt();
}


void _bounds(uint64_t regs,uint64_t error_code)
{
    printf("bounds -> %X\n", error_code);
    printsf("bounds -> %X\n", error_code);
    for(;;) hlt();
}

void _undefined_opcode(uint64_t regs,uint64_t error_code)
{
    printf("undefined_opcode -> %X\n", error_code);
    printsf("undefined_opcode -> %X\n", error_code);
    for(;;) hlt();
}

void _dev_not_available(uint64_t regs,uint64_t error_code)
{
    printf("dev_not_available -> %X\n", error_code);
    printsf("dev_not_available -> %X\n", error_code);
    for(;;) hlt();
}

void _double_fault(uint64_t regs, uint64_t error_code)
{
    printf("double_fault -> %X\n", error_code);
    printsf("double_fault -> %X\n", error_code);
    for(;;) hlt();
}

void _coprocessor_segment_overrun(uint64_t regs,uint64_t error_code)
{
    printf("coprocessor_segment_overrun -> %X\n", error_code);
    printsf("coprocessor_segment_overrun -> %X\n", error_code);
    for(;;) hlt();
}

void _invalid_tss(uint64_t regs,uint64_t error_code)
{
    printf("invalid_tss -> %X\n", error_code);
    printsf("invalid_tss -> %X\n", error_code);
    for(;;) hlt();
}

void _segment_not_present(uint64_t regs,uint64_t error_code)
{
    printf("segment_not_present -> %X\n", error_code);
    printsf("segment_not_present -> %X\n", error_code);
    for(;;) hlt();
}

void _stack_segment_fault(uint64_t regs,uint64_t error_code)
{
    printf("stack_segment_fault -> %X\n", error_code);
    printsf("stack_segment_fault -> %X\n", error_code);
    for(;;) hlt();
}

void _general_protection(uint64_t regs, uint64_t error_code)
{
    printf("general_protection -> 0x%X\n", error_code);
    printsf("general_protection -> 0x%X\n", error_code);
    for(;;) hlt();
}

void _page_fault(uint64_t regs,uint64_t error_code)
{
    printf("page_fault -> %X\n", error_code);
    printsf("page_fault -> %X\n", error_code);
    for(;;) hlt();
}

void _x87_fpu_error(uint64_t regs,uint64_t error_code)
{
    printf("x87_fpu_error -> %X\n", error_code);
    printsf("x87_fpu_error -> %X\n", error_code);
    for(;;) hlt();
}

void _alignment_check(uint64_t regs,uint64_t error_code)
{
    printf("alignment_check -> %X\n", error_code);
    printsf("alignment_check -> %X\n", error_code);
    for(;;) hlt();
}

void _machine_check(uint64_t regs,uint64_t error_code)
{
    printf("machine_check -> %X\n", error_code);
    printsf("machine_check -> %X\n", error_code);
    for(;;) hlt();
}

void _simd_exception(uint64_t regs,uint64_t error_code)
{
    printf("simd_exception -> %X\n", error_code);
    printsf("simd_exception -> %X\n", error_code);
    for(;;) hlt();
}

void _virtualization_exception(uint64_t regs,uint64_t error_code)
{
    printf("virtualization_exception -> %X\n", error_code);
    printsf("virtualization_exception -> %X\n", error_code);
    for(;;) hlt();
}
