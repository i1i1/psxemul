#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"


struct region {
	uint32_t start;
	uint32_t end;
	uint8_t (*getb)(uint32_t off);
	void (*setb)(uint32_t off, uint8_t val);
};


struct memory mem;


static uint8_t
mem_getb_user_mem(uint32_t off)
{
	return mem.user_mem[off];
}

static void
mem_setb_user_mem(uint32_t off, uint8_t val)
{
	mem.user_mem[off] = val;
}

static uint8_t
mem_getb_bios(uint32_t off)
{
	return mem.bios[off];
}

static void
mem_setb_bios(uint32_t off, uint8_t val)
{
	mem.bios[off] = val;
}

static uint8_t
mem_getb_memc1(uint32_t off)
{
	exit(1);
	switch (off) {
	case 0x00:
		return 0;
	}
}

static void
mem_setb_memc1(uint32_t off, uint8_t val)
{
	printf("off = %p\n", off);
//	exit(1);
//	mem.bios[off] = val;
}

struct region mmap[] = {
	{ 0x00010000, 0x00200000, mem_getb_user_mem,	mem_setb_user_mem },
	{ 0xbfc00000, 0xbfc80000, mem_getb_bios,	mem_setb_bios     },
	{ 0x1f801000, 0x1f801024, mem_getb_memc1,	mem_setb_memc1    },
};

uint8_t
mem_getb(uint32_t addr)
{
	for (int i = 0; i < (int)(sizeof(mmap)/sizeof(mmap[0])); i++) {
		if (mmap[i].start <= addr && addr < mmap[i].end)
			return mmap[i].getb(addr-mmap[i].start);
	}
	printf("Unknown address: 0x%8x\n", addr);
	exit(1);
}

void
mem_setb(uint32_t addr, uint8_t b)
{
	printf("setb %p %02x\n", (void *)(size_t)addr, b);
	for (int i = 0; i < (int)(sizeof(mmap)/sizeof(mmap[0])); i++) {
		if (mmap[i].start <= addr && addr < mmap[i].end) {
			mmap[i].setb(addr-mmap[i].start, b);
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

