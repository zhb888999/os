#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <int.h>

typedef struct {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t ds;
	uint64_t es;
	uint64_t rax;
	uint64_t func;
	uint64_t errcode;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} ExceptionRegs;

typedef struct {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t ds;
	uint64_t es;
	uint64_t rax;
	uint64_t func;
	uint64_t nr;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} IRQRegs;

typedef struct {
    void (*enable)(uint64_t irq);
    void (*disable)(uint64_t irq);
    uint64_t (*install)(uint64_t irq, void *arg);
    void (*uninstall)(uint64_t irq);
    void (*ack)(uint64_t irq);
} HWController;

typedef struct {
    HWController *controller;
    char *name;
    uint64_t parameter;
    void(*handler)(uint64_t nr, uint64_t parameter, IRQRegs *regs);
    uint64_t flags;
} IRQDescriptor;

#define NR_IRQS 24

int32_t register_irq(
    uint64_t irq,
    void *arg,
    void (*handler)(uint64_t nr, uint64_t parameter, IRQRegs *regs),
    uint64_t parameter,
    HWController *controller,
    char *name
);
int32_t unregister_irq(uint64_t irq);

void init_idt(void);

#endif