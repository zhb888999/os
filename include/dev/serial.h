#ifndef SERIAL_H
#define SERIAL_H

#define PORT 0x3f8

void init_serial(void);
void printsf(const char *format, ...);

#endif // SERIAL_H
