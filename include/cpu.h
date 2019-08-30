#ifndef _CPU_H_
#define _CPU_H_

#include "types.h"

#include "memory.h"

#define RESET_ADDR    0xbfc00000ul

struct cpu {
    u32 r[32];
    u32 hi;
    u32 lo;
    u32 pc;
    u32 ir;
};

extern struct cpu cpu;


void cpu_init(void);

void cpu_iter(void);

#endif /*_CPU_H_*/
