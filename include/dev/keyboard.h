#ifndef DEV_KEYBOARD_H
#include <int.h>
#include <asm/io.h>

#define KB_IRQ_NR 1
#define KB_BUF_SIZE 100

#define PAUSEBREAK	1
#define PRINTSCREEN	2
#define OTHERKEY	4
#define FLAG_BREAK	0x80

#define PORT_KB_DATA	0x60
#define PORT_KB_STATUS	0x64
#define PORT_KB_CMD	0x64

#define KBSTATUS_IBF	0x02
#define KBSTATUS_OBF	0x01

#define KBCMD_WRITE_CMD	0x60
#define KBCMD_READ_CMD	0x20

#define KB_INIT_MODE	0x47

#define  wait_keyboard_write()	while(inb(PORT_KB_STATUS) & KBSTATUS_IBF){}
#define  wait_keyboard_read()	while(inb(PORT_KB_STATUS) & KBSTATUS_OBF){}

void keyboard_init(void);
void keyboard_exit(void);

typedef struct{
    uint8_t *phead;
    uint8_t *ptail;
    int count;
    uint8_t buf[KB_BUF_SIZE];
} KeyBoardInput;

void analysis_keycode(void);
#endif 