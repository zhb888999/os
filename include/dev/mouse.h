#ifndef DEV_MOUSE_H
#define DEV_MOUSE_H

#include <int.h>

#define MOUSE_IRQ_NR 12

#define MOUSE_BUF_SIZE 100
#define KBCMD_SENDTO_MOUSE	0xd4
#define MOUSE_ENABLE		0xf4
#define MOUSE_INIT_MODE	0x47

#define PORT_MOUSE_DATA	0x60
#define PORT_MOUSE_STATUS	0x64
#define PORT_MOUSE_CMD	0x64

#define KBCMD_EN_MOUSE_INTFACE	0xa8

#define MOUSE_STATUS_IBF	0x02
#define MOUSE_STATUS_OBF	0x01

#define MOUSE_CMD_WRITE_CMD	0x60
#define MOUSE_CMD_READ_CMD	0x20

typedef struct{
    uint8_t *phead;
    uint8_t *ptail;
    int count;
    uint8_t buf[MOUSE_BUF_SIZE];
} MouseInput;

typedef struct {
    uint8_t byte0;
    int8_t byte1;
    int8_t byte2;
} MousePacket;

#define  wait_mouse_write()	while(inb(PORT_MOUSE_STATUS) & MOUSE_STATUS_IBF){}
#define  wait_mouse_read()	while(inb(PORT_MOUSE_STATUS) & MOUSE_STATUS_OBF){}

void mouse_init(void);

#endif