
.global  _start, multiboot_info, gdt64_table, gdt_ptr, idt_ptr, idt_table, stack_bottom, stack_top

.section .text
.code32
_start:
    jmp  header_end
    .align 8
header_start:
    .long 0xe85250d6
    .long 0
    .long  header_end - header_start
    .long  -(0xe85250d6 + (header_end - header_start))
    .short 0
    .short 0
    .long 8
header_end:
    movl $stack_top, %esp
    movl %ebx, multiboot_info

    call setup_page_tables
    call enable_paging

    lgdt gdt_ptr
    lidt idt_ptr

    ljmp $0x8, $long_mode_start

setup_page_tables:
    movl $p3_table, %eax
    or $0x3, %eax
    movl %eax, p4_table

    movl $0x0, %eax
    or $0x83, %eax
    movl %eax, p3_table

    ret

enable_paging:
    movl $p4_table, %eax
    movl %eax, %cr3

    movl %cr4, %eax
    or $0x20, %eax
    movl %eax, %cr4

    movl $0xc0000080, %ecx
    rdmsr
    or $0x100, %eax
    wrmsr

    movl %cr0, %eax
    or  $0x80000000, %eax
    movl %eax, %cr0

    ret 

.code64
long_mode_start:
    movw $0, %ax
    movw %ax, %ss
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    call kernel_main

    movq $0x2f592f412f4b2f41, %rax
    movq %rax, 0xb8040
    hlt

.section .data
multiboot_info:
    .long 0
.align 4096
p4_table:
    .fill 512, 8, 0
p3_table:
    .fill 512, 8, 0

gdt64_table:
    .quad 0
    .quad 0x0020980000000000 
    .fill 510,8, 0
gdt64_table_end:

idt_table:
    .fill 512, 8, 0
idt_table_end:

gdt_ptr:
    // .short 0x10 - 1
    .short gdt64_table - gdt64_table_end - 1
    .long gdt64_table

idt_ptr:
    .short idt_table - idt_table_end - 1
    .long idt_table

stack_bottom:
    .fill 2048, 8, 0
stack_top:
