#ifndef PROGRESS_TASK_H
#define PROGRESS_TASK_H

#include <int.h>
#include <kernel/config.h>

typedef struct {
    uint64_t rsp0;

    uint64_t rip;
    uint64_t rsp;

    uint64_t fs;
    uint64_t gs;

    uint64_t cr2;
    uint64_t trap_nr;
    uint64_t errcode;
} TThread;

typedef struct {
    uint64_t *p4;
    uint64_t _code, code_;
    uint64_t _data, data_;
    uint64_t _rodata, rodata_;
    uint64_t _brk, brk_;
    uint64_t _stack;
} TMM;

struct _Task{
    volatile int64_t state;
    uint64_t flags;
    TMM *mm;
    TThread *thread;
};
typedef struct _Task Task;

typedef union {
    Task task;
    uint64_t stack[STACK_SIZE/sizeof(uint64_t)];
} TaskUnion;

#define switch(prev, next) \
do{ \
    __asm__ __volatile__( \
        "pushq  %%rbp               \n\t" \
        "pushq  %%rax               \n\t" \
        "movq   %%rax, %0           \n\t" \
        "leaq   1f(%%rip), %%rax    \n\t" \
        "movq   %%rax, %1           \n\t" \
        "pushq  %3                  \n\t" \
        "jmp    __switch            \n\t" \
        "1:                         \n\t" \
        "popq   %%rax               \n\t" \
        "popq   %%rbp               \n\t" \
        :"=m"(prev->thread->rsp),"=m"(prev->thread->rip) \
        :"m"(next->thread->rsp),"m"(next->thread->rip),"D"(prev),"S"(next) \
        :"memory" \
    ); \
}while(0)

#endif