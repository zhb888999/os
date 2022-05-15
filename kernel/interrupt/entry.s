.global default_interrupt
default_interrupt:
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

    movq    $0x10, %rax
    movq    %rax, %ds
    movq    %rax, %es

    callq   _default_interrupt

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

exception_recover_and_return:
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

    movq    %rax, %ds
	popq	%rax
    movq    %rax, %es
    popq    %rax
    addq    $0x10, %rsp
    iretq

ret_from_exception:
.global ret_from_intr
ret_from_intr:
    jmp     exception_recover_and_return

.global divide_error
divide_error:
    pushq   $0
    pushq   %rax
    leaq    _divide_error(%rip), %rax
    xchgq   %rax, (%rsp)

error_code:
    pushq   %rax
    movq    %es, %rax
    pushq   %rax
    movq    %ds, %rax
    pushq   %rax
    xorq    %rax, %rax

    pushq   %rbp
    pushq   %rdi
    pushq   %rsi
    pushq   %rdx
    pushq   %rcx
    pushq   %rbx
    pushq   %r8
    pushq   %r9
    pushq   %r10
    pushq   %r11
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    cld
    movq    0x90(%rsp), %rsi 
    movq    0x88(%rsp), %rdx
    movq    $0x10, %rdi
    movq    %rdi, %ds
    movq    %rdi, %es

    movq    %rsp, %rdi
    callq   *%rdx
    jmp     ret_from_exception

.global debug
debug:
	pushq	$0
	pushq	%rax
	leaq	_debug(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global nmi
nmi:
    pushq   %rax
    cld
    pushq   %rax

    pushq %rax
    movq %es, %rax
    pushq %rax
    movq %ds, %rax
    pushq %rax
    xorq %rax, %rax

    pushq %rbp
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    pushq %rbx
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    movq $0x10, %rdx;
    movq %rdx, %ds;
    movq %rdx, %es;

    movq $0, %rsi
    movq %rsp, %rdi

    callq _nmi

    jmp exception_recover_and_return

.global int3
int3:
    pushq	$0
	pushq	%rax
	leaq	_int3(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global overflow
overflow:
	pushq	$0
	pushq	%rax
	leaq	_overflow(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global bounds
bounds:
	pushq	$0
	pushq	%rax
	leaq	_bounds(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global undefined_opcode
undefined_opcode:
	pushq	$0
	pushq	%rax
	leaq	_undefined_opcode(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global dev_not_available	/* need rewrite */
dev_not_available:
	pushq	$0
	pushq	%rax
	leaq	_dev_not_available(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code


.global double_fault
double_fault:
	pushq	%rax
	leaq	_double_fault(%rip),	%rax
	xchgq	%rax, (%rsp)
	jmp	error_code

.global coprocessor_segment_overrun
coprocessor_segment_overrun:
	pushq	$0
	pushq	%rax
	leaq	_coprocessor_segment_overrun(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global invalid_tss
invalid_tss:
	pushq	%rax
	leaq	_invalid_tss(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global segment_not_present
segment_not_present:
	pushq	%rax
	leaq	_segment_not_present(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global stack_segment_fault
stack_segment_fault:
	pushq	%rax
	leaq	_stack_segment_fault(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global general_protection
general_protection:
	pushq	%rax
	leaq	_general_protection(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global page_fault
page_fault:
	pushq	%rax
	leaq	_page_fault(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global x87_fpu_error
x87_fpu_error:
	pushq	$0
	pushq	%rax
	leaq	_x87_fpu_error(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global alignment_check
alignment_check:
	pushq	%rax
	leaq	_alignment_check(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global machine_check
machine_check:
	pushq	$0
	pushq	%rax
	leaq	_machine_check(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global simd_exception
simd_exception:
	pushq	$0
	pushq	%rax
	leaq	_simd_exception(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

.global virtualization_exception
virtualization_exception:
	pushq	$0
	pushq	%rax
	leaq	_virtualization_exception(%rip),	%rax
	xchgq	%rax,	(%rsp)
	jmp	error_code

/* IRQ */
.global irq33
irq33:
    pushq   %rax
    leaq    _irq33(%rip), %rax
    xchgq   %rax, (%rsp)

irq_save_all:
    pushq   %rax
    movq    %es, %rax
    pushq   %rax
    movq    %ds, %rax
    pushq   %rax
    xorq    %rax, %rax

    pushq   %rbp
    pushq   %rdi
    pushq   %rsi
    pushq   %rdx
    pushq   %rcx
    pushq   %rbx
    pushq   %r8
    pushq   %r9
    pushq   %r10
    pushq   %r11
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    cld
    movq    0x88(%rsp), %rdx
    movq    $0x10, %rdi
    movq    %rdi, %ds
    movq    %rdi, %es

    movq    %rsp, %rdi
    callq   *%rdx

irq_recover_and_return:
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

    movq    %rax, %ds
	popq	%rax
    movq    %rax, %es
    popq    %rax
    addq    $0x08, %rsp
    iretq

.global irq32
irq32:
    pushq   %rax
    leaq    _irq32(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq44
irq44:
    pushq   %rax
    leaq    _irq44(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all
