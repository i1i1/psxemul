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

struct reg {
	u8 *reg;
};

static struct reg memc1_arr[] = {
#define REG(OFF, NM) [OFF / 4] = { (void *)&(mem.mmc1.NM) },
	REG(0x00, E1_Base)
	REG(0x04, E2_Base)
	REG(0x08, E1_Delay)
	REG(0x0c, E3_Delay)
	REG(0x10, BIOS_ROM)
	REG(0x14, SPU_Delay)
	REG(0x18, CDC_Delay)
	REG(0x1c, E2_Delay)
	REG(0x20, Com_Delay)
#undef REG
};
static struct reg memc2_arr[] = {
#define REG(OFF, NM) [OFF / 4] = { (void *)&(mem.mmc2.NM) },
	REG(0x00, RAM_Size)
#undef REG
};

static u8
mem_getb_memc1(u32 off)
{
	return memc1_arr[off / 4].reg[off % 4];
}

static void
mem_setb_memc1(u32 off, u8 val)
{
	memc1_arr[off / 4].reg[off % 4] = val;
}

static u8
mem_getb_memc2(u32 off)
{
	return memc2_arr[off / 4].reg[off % 4];
}

static void
mem_setb_memc2(u32 off, u8 val)
{
	memc2_arr[off / 4].reg[off % 4] = val;
}

static u8
mem_getb_cache(u32 off)
{
	return mem.cache >> (8 * off);
}

static void
mem_setb_cache(u32 off, u8 val)
{
	mem.cache &= ~(MASK(8) << (off * 8));
	mem.cache |= (val << (off * 8));
}

static struct region mmap[] = {
	{ 0x00000000, 0x00200000, mem_getb_user_mem,	mem_setb_user_mem },
	{ 0x1f801000, 0x1f801024, mem_getb_memc1,	mem_setb_memc1    },
	{ 0x1f801060, 0x1f801064, mem_getb_memc2,	mem_setb_memc2    },
	{ 0xbfc00000, 0xbfc80000, mem_getb_bios,	mem_setb_bios     },
	{ 0xfffe0130, 0xfffe0134, mem_getb_cache,	mem_setb_cache    },
};

u8
mem_getb(u32 addr)
{
	for (int i = 0; i < (int)(sizeof(mmap)/sizeof(mmap[0])); i++) {
		if (mmap[i].start <= addr && addr < mmap[i].end)
			return mmap[i].getb(addr-mmap[i].start);
	}
	die("\tUnknown address: 0x%08x\n", addr);
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
	die("\tUnknown address: 0x%08x\n", addr);
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

