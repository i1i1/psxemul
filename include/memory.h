#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

struct memory {
	uint8_t bios[512 * 1024];
	uint8_t user_mem[0x00200000 - 0x00010000];
};

extern struct memory mem;


void mem_init(void);

void mem_setb(uint32_t addr, uint8_t b);

static inline void mem_seth(uint32_t addr, uint16_t h) {
	mem_setb(addr, h & 0xff);
	mem_setb(addr+1, h >> 8);
}

static inline void mem_setw(uint32_t addr, uint32_t w) {
	mem_seth(addr, w & 0xffff);
	mem_seth(addr+2, w >> 16);
}

uint8_t mem_getb(uint32_t addr);

static inline uint16_t mem_geth(uint32_t addr) {
	return ((uint16_t)mem_getb(addr)) |
		((uint16_t)mem_getb(addr+1) << 8);
}

static inline uint32_t mem_getw(uint32_t addr) {
	return ((uint32_t)mem_geth(addr)) |
		((uint32_t)mem_geth(addr+2) << 16);
}

#endif /* _MEMORY_H_ */

