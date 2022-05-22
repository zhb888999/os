#ifndef INTERRUPT_EXCEPTIONS_H
#define INTERRUPT_EXCEPTIONS_H

extern void default_interrupt(void);
extern void divide_error(void);
extern void debug(void);
extern void nmi(void);
extern void int3(void);
extern void overflow(void);
extern void bounds(void);
extern void undefined_opcode(void);
extern void dev_not_available(void);
extern void double_fault(void);
extern void coprocessor_segment_overrun(void);
extern void invalid_tss(void);
extern void segment_not_present(void);
extern void stack_segment_fault(void);
extern void general_protection(void);
extern void page_fault(void);
extern void x87_fpu_error(void);
extern void alignment_check(void);
extern void machine_check(void);
extern void simd_exception(void);
extern void virtualization_exception(void);

#endif // INTERRUPT_H