#include <dev/vga.h>
#include <asm/io.h>
#include <arg.h>

static uint16_t *buffer;
static uint32_t climit;
static uint32_t rlimit;

static uint32_t col=0;
static uint32_t row=0;

static void write_byte(uint8_t c);
static void itoa(char *buf, uint64_t divisor, uint64_t d);
static void newline(void);
static void clear_row(uint32_t row);
static void set_cursor(void);


void init_vga(uintptr_t addr, uint32_t _climit, uint32_t _rlimit) {
    buffer = (uint16_t *)addr;
    climit = _climit;
    rlimit = _rlimit;
    for(uint32_t i = 0; i < climit*rlimit; i++) *(buffer + i) = 0;
}

void write_byte(uint8_t ch) {
  switch (ch) {
    case '\n': newline(); break;
    default: 
      if (col >= climit) newline();
      *(buffer + row * climit + col) = (ATTRIBUTE << 8 | ch);
      col += 1;
      if (col + row * climit == rlimit * climit) newline();
      break;
  }
  set_cursor();
}

void newline(void) {
  if(row < rlimit - 1) {
    row += 1;
    col = 0;
    return;
  }
  for(uint32_t i = 1; i < rlimit; i++) 
    for(uint32_t j = 0; j < climit; j++)
      *(buffer + (i - 1) * climit + j) = *(buffer + i * climit + j);
  clear_row(rlimit - 1);
  col = 0;
}

inline void clear_row(uint32_t row) {
  for(uint32_t i = 0; i < climit; i++) 
    *(buffer + row * climit + i) = (ATTRIBUTE << 8 | ' ');
}

inline void set_cursor(void) {
  uint16_t cursor = col + row * climit;
  outb(0x0e, POS_PORT);
  outb(cursor >> 8, VAL_PORT);
  outb(0x0f, POS_PORT);
  outb(cursor << 8 >> 8, VAL_PORT);
}

void printf(const char *format, ...) {
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
