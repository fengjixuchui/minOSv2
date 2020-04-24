#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define UNUSED(x) ((void)x)

#define QUEUE_SIZE (1600)
struct queue_char {
    int head;
    int num;
    char data[QUEUE_SIZE];  // 80 char x 20 line
};

void halt(void);
void busy_loop(void);
void io_cli(void);
void io_sti(void);
void stihlt(void);
extern uint8_t io_in8(uint16_t port);
extern void io_out8(uint16_t port, uint8_t val);
extern uint32_t io_in32(uint16_t port);
extern void io_out32(uint16_t port, uint32_t val);
uint64_t mypow(uint64_t num, uint64_t p);
void *mymemset(void *buf, int ch, int n);
void *memcpy(void *buf1, const void *buf2, unsigned long n);
void init_bss(void);
int strncmp(const char *s1, const char *s2, unsigned long n);

#endif
