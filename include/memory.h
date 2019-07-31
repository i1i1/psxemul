#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "macro.h"
#include "types.h"


struct memory {
	u8 bios[512 * 1024];
	u8 user_mem[0x00200000 - 0x00010000];
};


extern struct memory mem;


void mem_init(void);

void mem_setb(u32 addr, u8 b);

u8 mem_getb(u32 addr);

void load_bios(char *s, struct memory *mem);


static inline void mem_seth(u32 addr, u16 h) {
	mem_setb(addr, h & MASK(8));
	mem_setb(addr+1, h >> 8);
}

static inline void mem_setw(u32 addr, u32 w) {
	mem_seth(addr, w & MASK(16));
	mem_seth(addr+2, w >> 16);
}

static inline u16 mem_geth(u32 addr) {
	return ((u16)mem_getb(addr)) |
		((u16)mem_getb(addr+1) << 8);
}

static inline u32 mem_getw(u32 addr) {
	return ((u32)mem_geth(addr)) |
		((u32)mem_geth(addr+2) << 16);
}

#endif /* _MEMORY_H_ */

