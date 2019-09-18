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

struct cop0 {
    u32 r[32];
};

extern struct cpu cpu;
extern struct cop0 cop0;


void cpu_init(void);

void cpu_iter(void);

#endif /*_CPU_H_*/
