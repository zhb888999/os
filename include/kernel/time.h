#ifndef TIME_H
#define TIME_H
#include <asm/io.h>

#define HPET_IRQ_NR 2

typedef struct {
    int second;
    int minute;
    int hour;
    int day;
    int month;
    int year;
} Time;

#define CMOS_READ(addr) ({ \
    outb(0x80 | addr, 0x70); \
    inb(0x71); \
})

void cmos_time(Time *time);
void hpet_init(void);

#endif