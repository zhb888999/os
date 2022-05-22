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
.global irq00
irq00:
    pushq   $0x00
    pushq   %rax
    leaq    __irq(%rip), %rax
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
    movq    0x90(%rsp), %rsi
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
    addq    $0x10, %rsp
    iretq

.global irq01
irq01:
    pushq   $0x01
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq02
irq02:
    pushq   $0x02
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq03
irq03:
    pushq   $0x03
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq04
irq04:
    pushq   $0x04
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq05
irq05:
    pushq   $0x05
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq06
irq06:
    pushq   $0x06
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq07
irq07:
    pushq   $0x07
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq08
irq08:
    pushq   $0x08
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq09
irq09:
    pushq   $0x09
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq0a
irq0a:
    pushq   $0x0a
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq0b
irq0b:
    pushq   $0x0b
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq0c
irq0c:
    pushq   $0x0c
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq0d
irq0d:
    pushq   $0x0d
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq0e
irq0e:
    pushq   $0x0e
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq0f
irq0f:
    pushq   $0x0f
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq10
irq10:
    pushq   $0x10
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq11
irq11:
    pushq   $0x11
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq12
irq12:
    pushq   $0x12
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq13
irq13:
    pushq   $0x13
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq14
irq14:
    pushq   $0x14
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq15
irq15:
    pushq   $0x15
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq16
irq16:
    pushq   $0x16
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all

.global irq17
irq17:
    pushq   $0x17
    pushq   %rax
    leaq    __irq(%rip), %rax
    xchgq   %rax, (%rsp)
	jmp irq_save_all
