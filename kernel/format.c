#include <arg.h>
#include <kernel/format.h>

static void itoa(char *buf, uint64_t divisor, uint64_t d);

void sprintf(char *buffer, const char *format, ...) {
  va_list valist;
  va_start(valist, format);

  uint8_t ch;
  char buf[100];

  while ((ch = *format++) != 0) {
      if (ch != '%') {
        *buffer++ = ch;
      } else {
        char *p;
        ch = *format++;
        switch (ch) {
          case '%':
            *buffer++ = '%';
            break;
          case 'c':
            *buffer++ = va_arg(valist, char);
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
            while(*p) *buffer++ = *p++;
            break;
          default:
            *buffer++ = *(va_arg(valist, char*));
            break;
        }
      }
    }
  *buffer = '\0';
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
