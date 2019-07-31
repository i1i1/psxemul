#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "macro.h"

#include "memory.h"


struct region {
	u32 start;
	u32 end;
	u8 (*getb)(u32 off);
	void (*setb)(u32 off, u8 val);
};


struct memory mem;


static u8
mem_getb_user_mem(u32 off)
{
	return mem.user_mem[off];
}

static void
mem_setb_user_mem(u32 off, u8 val)
{
	mem.user_mem[off] = val;
}

static u8
mem_getb_bios(u32 off)
{
	return mem.bios[off];
}

static void
mem_setb_bios(u32 off, u8 val)
{
	mem.bios[off] = val;
}

static u8
mem_getb_memc1(u32 off)
{
	die("\t\tmemc1 off = %d\n", off);
}

static void
mem_setb_memc1(u32 off, u8 val)
{
	die("\t\tmemc1 off = %d val = %d\n", off, val);
}

static struct region mmap[] = {
	{ 0x00010000, 0x00200000, mem_getb_user_mem,	mem_setb_user_mem },
	{ 0x1f801000, 0x1f801024, mem_getb_memc1,	mem_setb_memc1    },
	{ 0xbfc00000, 0xbfc80000, mem_getb_bios,	mem_setb_bios     },
};

u8
mem_getb(u32 addr)
{
	for (int i = 0; i < (int)(sizeof(mmap)/sizeof(mmap[0])); i++) {
		if (mmap[i].start <= addr && addr < mmap[i].end)
			return mmap[i].getb(addr-mmap[i].start);
	}
	die("\tUnknown address: 0x%8x\n", addr);
}

void
mem_setb(u32 addr, u8 b)
{
	for (int i = 0; i < (int)(sizeof(mmap)/sizeof(mmap[0])); i++) {
		if (mmap[i].start <= addr && addr < mmap[i].end) {
			mmap[i].setb(addr-mmap[i].start, b);
			return;
		}
	}
	die("\tUnknown address: 0x%8x\n", addr);
}

void
mem_init()
{
	memset(mem.bios, 0, sizeof(mem.bios));
}

void
load_bios(char *s, struct memory *mem)
{
	FILE *fp = fopen(s, "r");

	if (!fp)
		exit(1);

	int _ = fread(mem->bios, sizeof(mem->bios), 1, fp);
	(void) _;

	fclose(fp);
}

