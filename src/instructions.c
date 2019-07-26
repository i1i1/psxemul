#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "memory.h"
#include "instructions.h"

/*
void (*i_type)(uint8_t rs, uint8_t rt, uint16_t imm);
void (*j_type)(uint32_t target);
void (*r_type)(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func);
*/

static int
sext16(uint16_t h)
{
	return (int16_t)h;
}

static void 
todo(uint32_t target)
{
    (void)target;
    printf("TODO ir = %p!\n", (void *)(size_t)(cpu.ir));
    exit(1);
}

static void 
unknown(uint32_t target)
{
    (void)target;
    printf("UNKNOWN ir = %p!\n", (void *)(size_t)(cpu.ir));
    exit(1);
}

static void
op_lui(uint8_t rs, uint8_t rt, uint16_t imm)
{
	(void) rs;

	cpu.r[rt] = imm << 16;
}

static void
op_ori(uint8_t rs, uint8_t rt, uint16_t imm)
{
	cpu.r[rt] = cpu.r[rs] | imm;
}

static void
op_sw(uint8_t rs, uint8_t rt, uint16_t imm)
{
	mem_setw(cpu.r[rs] + sext16(imm), cpu.r[rt]);
}

struct instruction instrs[64] = {
	[0x00] = { "SPECIAL",	J_TYPE,	{ .j_type = todo	}},
	[0x01] = { "BCOND",	J_TYPE, { .j_type = todo	}},
	[0x02] = { "J",		J_TYPE, { .j_type = todo	}},
	[0x03] = { "JAL",	J_TYPE, { .j_type = todo	}},
	[0x04] = { "BEQ",	J_TYPE, { .j_type = todo	}},
	[0x05] = { "BNE",	J_TYPE, { .j_type = todo	}},
	[0x06] = { "BLEZ",	J_TYPE, { .j_type = todo	}},
	[0x07] = { "BGTZ",	J_TYPE, { .j_type = todo	}},
	[0x08] = { "ADDI",	J_TYPE, { .j_type = todo	}},
	[0x09] = { "ADDIU",	J_TYPE, { .j_type = todo	}},
	[0x0A] = { "SLTI",	J_TYPE, { .j_type = todo	}},
	[0x0B] = { "SLTIU",	J_TYPE, { .j_type = todo	}},
	[0x0C] = { "ANDI",	J_TYPE, { .j_type = todo	}},
	[0x0D] = { "ORI",	I_TYPE, { .i_type = op_ori	}},
	[0x0E] = { "XORI",	J_TYPE, { .j_type = todo	}},
	[0x0F] = { "LUI",	I_TYPE, { .i_type = op_lui	}},
	[0x10] = { "COP0",	J_TYPE, { .j_type = todo	}},
	[0x11] = { "COP1",	J_TYPE, { .j_type = todo	}},
	[0x12] = { "COP2",	J_TYPE, { .j_type = todo	}},
	[0x13] = { "COP3",	J_TYPE, { .j_type = todo	}},
	[0x14] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x15] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x16] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x17] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x18] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x19] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x1A] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x1B] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x1C] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x1D] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x1E] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x1F] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x20] = { "LB",	J_TYPE, { .j_type = todo	}},
	[0x21] = { "LH",	J_TYPE, { .j_type = todo	}},
	[0x22] = { "LWL",	J_TYPE, { .j_type = todo	}},
	[0x23] = { "LW",	J_TYPE, { .j_type = todo	}},
	[0x24] = { "LBU",	J_TYPE, { .j_type = todo	}},
	[0x25] = { "LHU",	J_TYPE, { .j_type = todo	}},
	[0x26] = { "LWR",	J_TYPE, { .j_type = todo	}},
	[0x27] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x28] = { "SB ",	J_TYPE, { .j_type = todo	}},
	[0x29] = { "SH",	J_TYPE, { .j_type = todo	}},
	[0x2A] = { "SWL",	J_TYPE, { .j_type = todo	}},
	[0x2B] = { "SW",	I_TYPE, { .i_type = op_sw	}},
	[0x2C] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x2D] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x2E] = { "SWR",	J_TYPE, { .j_type = todo	}},
	[0x2F] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x30] = { "LWC0",	J_TYPE, { .j_type = todo	}},
	[0x31] = { "LWC1",	J_TYPE, { .j_type = todo	}},
	[0x32] = { "LWC2",	J_TYPE, { .j_type = todo	}},
	[0x33] = { "LWC3",	J_TYPE, { .j_type = todo	}},
	[0x34] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x35] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x36] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x37] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x38] = { "SWC0",	J_TYPE, { .j_type = todo	}},
	[0x39] = { "SWC1",	J_TYPE, { .j_type = todo	}},
	[0x3A] = { "SWC2",	J_TYPE, { .j_type = todo	}},
	[0x3B] = { "SWC3",	J_TYPE, { .j_type = todo	}},
	[0x3C] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x3D] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x3E] = { NULL,	J_TYPE, { .j_type = unknown	}},
	[0x3F] = { NULL,	J_TYPE, { .j_type = unknown	}},
};

