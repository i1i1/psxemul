#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"


struct memory mem;

struct region {
	uint32_t start;
	uint32_t end;
	uint8_t *buf;
};

struct region mmap[] = {
	{ 0x00010000, 0x00200000, mem.user_mem },
	{ 0xbfc00000, 0xbfc80000, mem.bios },
};

uint8_t
mem_getb(uint32_t addr)
{
	for (int i = 0; i < (int)(sizeof(mmap)/sizeof(mmap[0])); i++) {
		if (mmap[i].start <= addr && addr < mmap[i].end)
			return mmap[i].buf[addr-mmap[i].start];
	}
	printf("Unknown address: 0x%8x\n", addr);
	exit(1);
}

void
mem_setb(uint32_t addr, uint8_t b)
{
	for (int i = 0; i < (int)(sizeof(mmap)/sizeof(mmap[0])); i++) {
		if (mmap[i].start <= addr && addr < mmap[i].end) {
			mmap[i].buf[addr-mmap[i].start] = b;
			return;
		}
	}
	printf("Unknown address: 0x%8x\n", addr);
	exit(1);
}

void
mem_init()
{
	memset(mem.bios, 0, sizeof(mem.bios));
}

