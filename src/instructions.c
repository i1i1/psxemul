#include <stdio.h>
#include <stdlib.h>

#include "macro.h"
#include "types.h"

#include "cpu.h"
#include "memory.h"
#include "instructions.h"

/*
void (*i_type)(u8 rs, u8 rt, u16 imm);
void (*j_type)(u32 target);
void (*r_type)(u8 rs, u8 rt, u8 rd, u8 shamt, u8 func);
*/

static int
sext16(u16 h)
{
	return (int16_t)h;
}

static void 
todo(u32 target)
{
    (void)target;
    die("TODO ir = 0x%x! id = 0x%x \"%s\"\n", cpu.ir, cpu.ir >> 26, instrs[cpu.ir >> 26].nm);
}

static void 
unknown(u32 target)
{
    (void)target;
    die("Unknown ir = 0x%x!\n", cpu.ir);
}

static void
op_lui(u8 rs, u8 rt, u16 imm)
{
	(void) rs;

	cpu.r[rt] = imm << 16;
}

static void
op_ori(u8 rs, u8 rt, u16 imm)
{
	cpu.r[rt] = cpu.r[rs] | imm;
}

static void
op_sw(u8 rs, u8 rt, u16 imm)
{
	mem_setw(cpu.r[rs] + sext16(imm), cpu.r[rt]);
}

static void
op_special(u8 rs, u8 rt, u8 rd, u8 shamt, u8 func)
{
	(void) rs;

	enum {
		FUNC_SLL = 0x00,
		FUNC_SRL = 0x02,
		FUNC_SRA = 0x03,
		FUNC_OR  = 0x25,
		/*TODO*/
	};

	switch (func) {
	case FUNC_SLL:
		cpu.r[rd] = cpu.r[rt] << shamt;
		break;
	case FUNC_OR:
		cpu.r[rd] = cpu.r[rt] | cpu.r[rs];
		break;
	default:
		die("TODO func 0x%x\n", func);
	}
}

static void
op_j(u32 target)
{
	cpu.pc &= MASK_BITS(32, 28);
	cpu.pc |= (target << 2);
}

/*
 * TODO: Figure out traps and overflows!
 */
static void
op_addiu(u8 rs, u8 rt, u16 imm)
{
	cpu.r[rs] = cpu.r[rt] + sext16(imm);
}



struct instruction instrs[64] = {
#define R(idx, nm, func) [idx] = { nm, R_TYPE, { .r_type = func } },
#define J(idx, nm, func) [idx] = { nm, J_TYPE, { .j_type = func } },
#define I(idx, nm, func) [idx] = { nm, I_TYPE, { .i_type = func } },

	R  (  0x00,    "SPECIAL",	op_special	)
	J  (  0x01,    "BCOND",		todo		)
	J  (  0x02,    "J",		op_j		)
	J  (  0x03,    "JAL",		todo		)
	J  (  0x04,    "BEQ",		todo		)
	J  (  0x05,    "BNE",		todo		)
	J  (  0x06,    "BLEZ",		todo		)
	J  (  0x07,    "BGTZ",		todo		)
	J  (  0x08,    "ADDI",		todo		)
	I  (  0x09,    "ADDIU",		op_addiu	)
	J  (  0x0A,    "SLTI",		todo		)
	J  (  0x0B,    "SLTIU",		todo		)
	J  (  0x0C,    "ANDI",		todo		)
	I  (  0x0D,    "ORI",		op_ori		)
	J  (  0x0E,    "XORI",		todo		)
	I  (  0x0F,    "LUI",		op_lui		)
	J  (  0x10,    "COP0",		todo		)
	J  (  0x11,    "COP1",		todo		)
	J  (  0x12,    "COP2",		todo		)
	J  (  0x13,    "COP3",		todo		)
	J  (  0x14,    NULL,		unknown		)
	J  (  0x15,    NULL,		unknown		)
	J  (  0x16,    NULL,		unknown		)
	J  (  0x17,    NULL,		unknown		)
	J  (  0x18,    NULL,		unknown		)
	J  (  0x19,    NULL,		unknown		)
	J  (  0x1A,    NULL,		unknown		)
	J  (  0x1B,    NULL,		unknown		)
	J  (  0x1C,    NULL,		unknown		)
	J  (  0x1D,    NULL,		unknown		)
	J  (  0x1E,    NULL,		unknown		)
	J  (  0x1F,    NULL,		unknown		)
	J  (  0x20,    "LB",		todo		)
	J  (  0x21,    "LH",		todo		)
	J  (  0x22,    "LWL",		todo		)
	J  (  0x23,    "LW",		todo		)
	J  (  0x24,    "LBU",		todo		)
	J  (  0x25,    "LHU",		todo		)
	J  (  0x26,    "LWR",		todo		)
	J  (  0x27,    NULL,		unknown		)
	J  (  0x28,    "SB ",		todo		)
	J  (  0x29,    "SH",		todo		)
	J  (  0x2A,    "SWL",		todo		)
	I  (  0x2B,    "SW",		op_sw		)
	J  (  0x2C,    NULL,		unknown		)
	J  (  0x2D,    NULL,		unknown		)
	J  (  0x2E,    "SWR",		todo		)
	J  (  0x2F,    NULL,		unknown		)
	J  (  0x30,    "LWC0",		todo		)
	J  (  0x31,    "LWC1",		todo		)
	J  (  0x32,    "LWC2",		todo		)
	J  (  0x33,    "LWC3",		todo		)
	J  (  0x34,    NULL,		unknown		)
	J  (  0x35,    NULL,		unknown		)
	J  (  0x36,    NULL,		unknown		)
	J  (  0x37,    NULL,		unknown		)
	J  (  0x38,    "SWC0",		todo		)
	J  (  0x39,    "SWC1",		todo		)
	J  (  0x3A,    "SWC2",		todo		)
	J  (  0x3B,    "SWC3",		todo		)
	J  (  0x3C,    NULL,		unknown		)
	J  (  0x3D,    NULL,		unknown		)
	J  (  0x3E,    NULL,		unknown		)
	J  (  0x3F,    NULL,		unknown		)
#undef J
#undef I
#undef R
};

