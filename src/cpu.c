#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "macro.h"

#include "cpu.h"
#include "memory.h"
#include "instructions.h"


struct cpu cpu;


void
cpu_init()
{
	cpu.pc = RESET_ADDR;
}

void
cpu_iter()
{
	u32 inst = mem_getw(cpu.pc);
	u32 op = inst >> 26;

	cpu.ir = inst;

	printf("%x:\t%x\t%s\t", cpu.pc, inst, instrs[op].nm);

	switch (instrs[op].type) {
	case J_TYPE: {
		u32 target = inst & MASK(26);

		printf("0x%x\n", target);
		instrs[op].f.j_type(target);
		break;
	}
	case I_TYPE: {
		u8  rs  = (inst >> 21) & MASK(5);
		u8  rt  = (inst >> 16) & MASK(5);
		u16 imm = (inst >>  0) & MASK(16);

		printf("0x%x, 0x%x, 0x%x\n", rs, rt, imm);
		instrs[op].f.i_type(rs, rt, imm);
		break;
	}
	case R_TYPE: {
		u8 rs    = (inst >> 21) & MASK(5);
		u8 rt    = (inst >> 16) & MASK(5);
		u8 rd    = (inst >> 11) & MASK(5);
		u8 shamt = (inst >>  6) & MASK(5);
		u8 func  = (inst >>  0) & MASK(6);

		printf("0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", rs, rt, rd, shamt, func);
		instrs[op].f.r_type(rs, rt, rd, shamt, func);
		break;
	}
	}

	cpu.pc += 4;
}

