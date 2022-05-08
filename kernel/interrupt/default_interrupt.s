.global default_interrupt; default_interrupt:
    cld
    pushq   %rax
    pushq   %rbx
    pushq   %rcx
    pushq   %rdx
    pushq   %rbp
    pushq   %rdi
    pushq   %rsi

    pushq   %r8
    pushq   %r9
    pushq   %r10
    pushq   %r11
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    movq    %es, %rax 
    pushq   %rax
    movq    %ds, %rax
    pushq   %rax

    leaq    int_msg(%rip), %rax
    pushq   %rax
    movq    %rax, %rdi
    movq    $0, %rax
    callq   printf
    addq    $0x80, %rsp

loop:
    hlt
    jmp loop

    popq    %rax
    movq    %rax, %ds
    popq    %rax
    movq    %rax, %es

    popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8

	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%rdx
	popq	%rcx
	popq	%rbx
	popq	%rax
	iretq

int_msg:
	.asciz "Unknown interrupt or fault at RIP\n"