#include <int.h>
#include <dev/vga.h>
#include <dev/serial.h>

void init_local_apic(void) {
    uint32_t hmsr, lmsr;

    /* enable xAPIC & x2APIC */
    __asm__ __volatile__(
        "movq   $0x1b, %%rcx    \n\t"
        "rdmsr                  \n\t"
        "bts    $10, %%rax      \n\t"
        "bts    $11, %%rax      \n\t"
        "wrmsr                  \n\t"
        "movq   $0x1b, %%rcx    \n\t"
        "rdmsr                  \n\t"
        :"=a"(lmsr),"=d"(hmsr)
        :
        :"memory"
    );
    /* enable SVR[8] */
    __asm__ __volatile__(
        "movq   $0x80f, %%rcx   \n\t"
        "rdmsr                  \n\t"
        "bts    $8, %%rax       \n\t"
        "bts    $12, %%rax      \n\t"
        "wrmsr                  \n\t"
        "movq   $0x80f, %%rcx    \n\t"
        "rdmsr                  \n\t"
        :"=a"(lmsr),"=d"(hmsr)
        :
        :"memory"
    );

    /* get local APIC ID */
    __asm__ __volatile__(
        "movq   $0x802, %%rcx   \n\t"
        "rdmsr                  \n\t"
        :"=a"(lmsr),"=d"(hmsr)
        :
        :"memory"
    );
    printsf("apic id:%x\n", lmsr);

    /* get local APIC version */
    __asm__ __volatile__(
        "movq   $0x803, %%rcx   \n\t"
        "rdmsr                  \n\t"
        :"=a"(lmsr),"=d"(hmsr)
        :
        :"memory"
    );
	printsf("local APIC Version:0x%x, Max LVT Entry:0x%x,SVR(Suppress EOI Broadcast):0x%x\n",
           lmsr & 0xff,(lmsr >> 16 & 0xff) + 1,lmsr >> 24 & 0x1);
    
    return;
    /* mask all LVT */
    __asm__ __volatile__(
        "movq   $0x82f, %%rcx   \n\t"   //CMCI
        "wrmsr                  \n\t"
        "movq   $0x832, %%rcx   \n\t"   //Timer
        "wrmsr                  \n\t"
        "movq   $0x833, %%rcx   \n\t"   //Thermal Monitor
        "wrmsr                  \n\t"
        "movq   $0x834, %%rcx   \n\t"   //Performance Counter
        "wrmsr                  \n\t"
        "movq   $0x835, %%rcx   \n\t"   //LINT0
        "wrmsr                  \n\t"
        "movq   $0x836, %%rcx   \n\t"   //LINT1
        "wrmsr                  \n\t"
        "movq   $0x837, %%rcx   \n\t"   //Error
        "wrmsr                  \n\t"
        :
        :"a"(0x10000),"d"(0x00)
        :"memory"
    );

    /* get TPR */
    __asm__ __volatile__(
        "movq   $0x808, %%rcx   \n\t"
        "rdmsr                  \n\t"
        :"=a"(lmsr),"=d"(hmsr)
        :
        :"memory"
    );
    printf("set LVT TPR:0x%x", lmsr);
    /* get PPR */
    __asm__ __volatile__(
        "movq   $0x80a, %%rcx   \n\t"
        "rdmsr                  \n\t"
        :"=a"(lmsr),"=d"(hmsr)
        :
        :"memory"
    );
    printf("set LVT PPR:0x%x", lmsr);

}