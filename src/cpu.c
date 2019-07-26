#include <stdio.h>
#include <stdlib.h>

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
	uint32_t inst = mem_getw(cpu.pc);
	uint32_t op = inst >> 26;

	cpu.ir = inst;

	switch (instrs[op].type) {
	case J_TYPE: {
		uint32_t target = inst & 0x03ffffff;  // Last 6 bits are opcode

		printf("%p:\t%p\t%s\t%p\n", (void *)(size_t)cpu.pc, (void *)(size_t)inst, instrs[op].nm, (void *)(size_t)target);
		instrs[op].f.j_type(target);
		break;
	}
	case I_TYPE: {
		uint8_t  rs  = (inst >> 21) & 0x1f;
		uint8_t  rt  = (inst >> 16) & 0x1f;
		uint16_t imm = (inst >>  0) & 0xffff;

		printf("%p:\t%p\t%s\t%d, %d, %d\n", (void *)(size_t)cpu.pc, (void *)(size_t)inst, instrs[op].nm, rs, rt, imm);
		instrs[op].f.i_type(rs, rt, imm);
		break;
	}
	case R_TYPE: {
		uint8_t rs    = (inst >> 21) & 0x1f;
		uint8_t rt    = (inst >> 16) & 0x1f;
		uint8_t rd    = (inst >> 11) & 0x1f;
		uint8_t shamt = (inst >>  6) & 0x1f;
		uint8_t func  = (inst >>  0) & 0x3f;

		printf("%p:\t%p\t%s\t%d, %d, %d, %d, %d\n", (void *)(size_t)cpu.pc, (void *)(size_t)inst, instrs[op].nm, rs, rt, rd, shamt, func);
		instrs[op].f.r_type(rs, rt, rd, shamt, func);
		break;
	}
	}

	cpu.pc += 4;
}

