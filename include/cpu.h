#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

#include "memory.h"

#define RESET_ADDR	0xbfc00000

struct cpu {
	uint32_t r[32];
	uint32_t hi;
	uint32_t lo;
	uint32_t pc;
	uint32_t ir;
};

extern struct cpu cpu;


void cpu_init(void);

void cpu_iter(void);

#endif /*_CPU_H_*/
