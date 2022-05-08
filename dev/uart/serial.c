#include <asm/io.h>
#include <dev/serial.h>
#include <arg.h>
#include <int.h>

void init_serial() {
    outb(0x00, PORT + 1);    // Disable all interrupts
    outb(0x80, PORT + 3);    // Enable DLAB (set baud rate divisor)
    outb(0x03, PORT + 0);    // Set divisor to 3 (lo byte) 38400 baud
    outb(0x00, PORT + 1);    //                  (hi byte)
    outb(0x03, PORT + 3);    // 8 bits, no parity, one stop bit
    outb(0xC7, PORT + 2);    // Enable FIFO, clear them, with 14-byte threshold
    outb(0x0B, PORT + 4);    // IRQs enabled, RTS/DSR set
};

static void itoa(char *buf, uint64_t divisor, uint64_t d);

/*  Put the character C to serial. */
void write_byte(uint8_t a) {
   while ((inb(PORT + 5) & 0x20) == 0);
   outb(a, PORT);
};


void printsf(const char *format, ...) {
  va_list valist;
  va_start(valist, format);

  uint8_t ch;
  char buf[100];

  while ((ch = *format++) != 0) {
      if (ch != '%') {
        write_byte(ch);
      } else {
        char *p;
        ch = *format++;
        switch (ch) {
          case '%':
            write_byte(ch);
            break;
          case 'c':
            write_byte(va_arg(valist, uint32_t));
            break;
          case 'b':
            itoa(buf, 2, va_arg(valist, uint32_t));
            p = buf;
            goto string;
            break;
          case 'B':
            itoa(buf, 2, va_arg(valist, uint64_t));
            p = buf;
            goto string;
            break;
          case 'd':
            itoa(buf, 10, va_arg(valist, uint32_t));
            p = buf;
            goto string;
            break;
          case 'D':
            itoa(buf, 10, va_arg(valist, uint64_t));
            p = buf;
            goto string;
            break;
          case 'x':
            itoa(buf, 16, va_arg(valist, uint32_t));
            p = buf;
            goto string;
            break;
          case 'X':
            itoa(buf, 16, va_arg(valist, uint64_t));
            p = buf;
            goto string;
            break;
          case 's':
            p = va_arg(valist, char *);
            if(!p) p = "NULL";
          string:
            while(*p) write_byte(*p++);
            break;
          default:
            write_byte(*(va_arg(valist, char*)));
            break;
        }
      }
    }
  va_end(valist);
}

static void itoa (char *buf, uint64_t divisor, uint64_t d)  {
    char *p = buf;
    char *p1, *p2;
    uint64_t ud = d;
    /*  Divide UD by DIVISOR until UD == 0. */
    do {
        uint64_t remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
     } while (ud /= divisor);

    /*  Terminate BUF. */
    *p = '\0';

    /*  Reverse BUF. */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}
