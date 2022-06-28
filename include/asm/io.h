#ifndef IO_H
#define IO_H

#define outw(value,port) __asm__ __volatile__("outw %%ax,%%dx;mfence;" ::"ax"(value),"dx"(port))
#define outl(value,port) __asm__ __volatile__("outl %%eax,%%dx;mfence;" ::"eax"(value),"dx"(port))
#define outb(value,port) __asm__ __volatile__("outb %%al,%%dx;mfence;"::"a" (value),"d" (port))
#define inw(port) \
({	\
    unsigned short int v; \
    asm volatile ("inw %%dx,%%ax" \
                    :"=a" (v):"dx" (port) \
                    :); \
    v; \
})
#define inl(port) \
({	\
unsigned int v; \
    asm volatile ("inl %%dx,%%eax" \
                    :"=a" (v):"dx" (port) \
                    :); \
v; \
})

#define inb(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
_v; \
})

#define outb_p(value,port) \
__asm__ ("outb %%al,%%dx\n" \
        "\tjmp 1f\n" \
        "1:\tjmp 1f\n" \
        "1:"::"a" (value),"d" (port))

#define inb_p(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al\n" \
    "\tjmp 1f\n" \
    "1:\tjmp 1f\n" \
    "1:":"=a" (_v):"d" (port)); \
_v; \
})

#define insb(buffer, nr, port)	\
__asm__ __volatile__("cld;rep;insb;mfence;"::"d"(port),"D"(buffer),"c"(nr):"memory")

#define insw(buffer, nr, port)	\
__asm__ __volatile__("cld;rep;insw;mfence;"::"d"(port),"D"(buffer),"c"(nr):"memory")

#define insq(buffer, nr, port)	\
__asm__ __volatile__("cld;rep;insq;mfence;"::"d"(port),"D"(buffer),"c"(nr):"memory")

#define outsb(buffer, nr, port)	\
__asm__ __volatile__("cld;rep;outsb;mfence;"::"d"(port),"S"(buffer),"c"(nr):"memory")

#define outsw(buffer, nr, port)	\
__asm__ __volatile__("cld;rep;outsw;mfence;"::"d"(port),"S"(buffer),"c"(nr):"memory")

#define outsq(buffer, nr, port)	\
__asm__ __volatile__("cld;rep;outsq;mfence;"::"d"(port),"S"(buffer),"c"(nr):"memory")

inline void * memcpy(void *From,void * To,long Num)
{
	int d0,d1,d2;
	__asm__ __volatile__	(	"cld	\n\t"
					"rep	\n\t"
					"movsq	\n\t"
					"testb	$4,%b4	\n\t"
					"je	1f	\n\t"
					"movsl	\n\t"
					"1:\ttestb	$2,%b4	\n\t"
					"je	2f	\n\t"
					"movsw	\n\t"
					"2:\ttestb	$1,%b4	\n\t"
					"je	3f	\n\t"
					"movsb	\n\t"
					"3:	\n\t"
					:"=&c"(d0),"=&D"(d1),"=&S"(d2)
					:"0"(Num/8),"q"(Num),"1"(To),"2"(From)
					:"memory"
				);
	return To;
}
#endif // IO_H