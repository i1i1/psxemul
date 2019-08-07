#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "macro.h"
#include "types.h"


struct memory {
	u8 bios[512 * 1024];
	u8 user_mem[0x00200000 - 0x00010000];
	struct mmc1 {
		u32 E1_Base;   // 1F801000h Expansion 1 Base Address (usually 1F000000h)
		u32 E2_Base;   // 1F801004h Expansion 2 Base Address (usually 1F802000h)
		u32 E1_Delay;  // 1F801008h Expansion 1 Delay/Size (usually 0013243Fh; 512Kbytes 8bit-bus)
		u32 E3_Delay;  // 1F80100Ch Expansion 3 Delay/Size (usually 00003022h; 1 byte)
		u32 BIOS_ROM;  // 1F801010h BIOS ROM    Delay/Size (usually 0013243Fh; 512Kbytes 8bit-bus)
		u32 SPU_Delay; // 1F801014h SPU_DELAY   Delay/Size (usually 200931E1h)
		u32 CDC_Delay; // 1F801018h CDROM_DELAY Delay/Size (usually 00020843h or 00020943h)
		u32 E2_Delay;  // 1F80101Ch Expansion 2 Delay/Size (usually 00070777h; 128-bytes 8bit-bus)
		u32 Com_Delay; // 1F801020h COM_DELAY / COMMON_DELAY (00031125h or 0000132Ch or 00001325h)
	} mmc1;
	struct mmc2 {
		u32 RAM_Size;  // 1F801060h RAM_SIZE (usually 00000B88h; 2MB RAM mirrored in first 8MB)
	} mmc2;
	u32 cache_ctl;
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

