#include <arch/x86_64.h>
#include <kernel/progress/task.h>
#include <int.h>

extern TSS64 tss;

TSS64 init_tss[1];

inline void set_task(Task *task, uint64_t state, uint64_t flages, TMM *mm, TThread *thread) {
    task->state = state;
    task->flags = flages;
    task->mm = mm;
    task->thread = thread;
}

inline void __switch(Task *prev, Task *next) {
    init_tss[0].rsp0 = next->thread->rsp0;
    set_tss64(&tss, init_tss[0].rsp0, init_tss[0].rsp1, init_tss[0].rsp2, 
    init_tss[0].ist1, init_tss[0].ist2, init_tss[0].ist3, init_tss[0].ist4, 
    init_tss[0].ist5, init_tss[0].ist6, init_tss[0].ist7);
    __asm__ __volatile__("movq %%fs, %0 \n\t":"=a"(prev->thread->fs));
    __asm__ __volatile__("movq %%gs, %0 \n\t":"=a"(prev->thread->gs));

    __asm__ __volatile__("movq %0, %%fs \n\t"::"a"(next->thread->fs));
    __asm__ __volatile__("movq %0, %%gs \n\t"::"a"(next->thread->gs));
}