#include <dev/vga.h>
#include <asm/io.h>
#include <arch/x86_64.h>
#include <kernel/interrupt.h>

IRQDescriptor irq_descs[NR_IRQS] = {0};

void __irq(IRQRegs *regs, uint64_t nr) {
    printf("@ irq[%D|0x%X] in\n", nr, nr);
    IRQDescriptor *irq = &irq_descs[nr];
    if(irq->handler != 0) {
        printf("@ irq[%D|0x%X] handler\n", nr, nr);
        irq->handler(nr, irq->parameter, regs);
    }
    
    if(irq->controller != 0 && irq->controller->ack != 0) {
        printf("@ irq[%D|0x%X] ack\n", nr, nr);
        irq->controller->ack(nr);
    }
    printf("@ irq[%D|0x%X] out\n", nr, nr);
    // __asm__ __volatile__(	
    //     "movq	$0x00,	%%rdx	\n\t" 
	// 	"movq	$0x00,	%%rax	\n\t" 
	// 	"movq 	$0x80b,	%%rcx	\n\t" 
	// 	"wrmsr	\n\t" 
	// 	:::"memory" 
    // ); 
}

int32_t register_irq(
    uint64_t irq,
    void *arg,
    void (*handler)(uint64_t nr, uint64_t parameter, IRQRegs *regs),
    uint64_t parameter,
    HWController *controller,
    char *name
) {
    IRQDescriptor *p = &irq_descs[irq];
    p->controller = controller;
    p->handler = handler;
    p->name = name;
    p->parameter = parameter;
    p->flags = 0;

    p->controller->install(irq, arg);
    p->controller->enable(irq);

    printf("@ register irq[%D|0x%X] controller:0x%X handler:0x%X\n", 
            irq, irq, (uint64_t)controller, (uint64_t)handler);
    return 1;
}

int32_t unregister_irq(uint64_t irq) {
    IRQDescriptor *p = &irq_descs[irq];
    p->controller->disable(irq);
    p->controller->uninstall(irq);

    p->controller = 0;
    p->name = 0;
    p->flags = 0;
    p->handler = 0;

    return 1;
}
