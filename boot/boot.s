
.global  _start, multiboot_info, p4_table, p3_table, gdt64_table, gdt_ptr, idt_ptr, idt_table, stack_bottom, stack_top

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

    movl $0x83, p3_table            
    movl $0x40000083, p3_table + 8  
    movl $0x80000083, p3_table + 16 
    movl $0xc0000083, p3_table + 24 
    movl $0x83, p3_table + 32
    movl $0x1, p3_table + 36
    movl $0x40000083, p3_table + 40 
    movl $0x1, p3_table + 44
    movl $0x80000083, p3_table + 48 
    movl $0x1, p3_table + 52
    movl $0xc0000083, p3_table + 56
    movl $0x1, p3_table + 60

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

    movq $0x1b, %rcx
    rdmsr
    bt $8, %rax
    jnc start_smp
    call kernel_main

    hlt

start_smp:
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


# boot APU
.section .text
.code16
.global apu_start
apu_start:
apu_base = .
    cli
    wbinvd
    mov %cs, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %ss
	mov	%ax, %fs
	mov	%ax, %gs

#	load idt gdt
	
	lidtl	apu_tmp_idt - apu_base
	lgdtl	apu_tmp_gdt_ptr - apu_base

#	enable protected mode

	smsw	%ax
	bts	$0	,%ax
	lmsw	%ax

#	go to 32 code
	ljmpl	*(apu_code32_vector - apu_base)

.code32
.balign 4
apu_code32:
#	go to 64 code
	mov	$0x10,	%ax
	mov	%ax,	%ds
	mov	%ax,	%es
	mov	%ax,	%ss
	mov	%ax,	%fs
	mov	%ax,	%gs

	leal (apu_tmp_stack_end - apu_base)(%esi),	%eax
	movl %eax, %esp

#	open PAE
 	movl $p4_table, %eax
 	movl %eax, %cr3

 	movl %cr4, %eax
    or $0x20, %eax
 	movl %eax, %cr4

	movl $0xC0000080, %ecx
	rdmsr
	or $0x100, %eax
	wrmsr

	movl %cr0, %eax
    or $0x80000000, %eax
	movl %eax,	%cr0

    lgdt gdt_ptr
    lidt idt_ptr

    ljmp $0x08, $long_mode_start
	

.balign 4
apu_tmp_idt:
	.word	0
	.word	0, 0

apu_tmp_gdt_ptr:
 	.short	apu_tmp_gdt_end - apu_tmp_gdt - 1
 	.long	apu_tmp_gdt - apu_base

.balign 4
apu_tmp_gdt:
 	.quad	0
 	.quad	0x00cf9e000000ffff
 	.quad	0x00cf92000000ffff

apu_tmp_gdt_end:

.balign 4
apu_code32_vector:
	.long	apu_code32 - apu_base
	.word	0x08, 0


.balign 4
apu_tmp_stack_start:
    .fill 512, 8, 0
apu_tmp_stack_end: 

.global apu_end
apu_end: