#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"

#include "cpu.h"
#include "memory.h"
#include "instructions.h"


struct cpu cpu;
struct cop0 cop0;


void
cpu_init()
{
    cpu.pc = RESET_ADDR;
}

static void
dump_regs()
{
    for (int i = 0; i < 8; i++) {
        printf("\t");
        for (int j = 0; j < 4; j++)
            printf("[%02x] = %08x;\t", i*4+j, cpu.r[i*4+j]);
        printf("\n");
    }
}

void
cpu_iter()
{
    u32 inst = mem_getw(cpu.pc);
    u32 op = inst >> 26;

    cpu.ir = inst;

    dump_regs();
    printf("%08x:\t%08x\t%s\t", cpu.pc, inst, instrs[op].nm);

    switch (instrs[op].type) {
    case J_TYPE: {
        u32 target = inst & MASK(26);

        printf("0x%08x\n", target);
        instrs[op].f.j_type(target);
        break;
    }
    case I_TYPE: {
        u8  rs  = (inst >> 21) & MASK(5);
        u8  rt  = (inst >> 16) & MASK(5);
        u16 imm = (inst >>  0) & MASK(16);

        printf("r%d, r%d, 0x%x\n", rs, rt, imm);
        instrs[op].f.i_type(rs, rt, imm);
        break;
    }
    case R_TYPE: {
        u8 rs    = (inst >> 21) & MASK(5);
        u8 rt    = (inst >> 16) & MASK(5);
        u8 rd    = (inst >> 11) & MASK(5);
        u8 shamt = (inst >>  6) & MASK(5);
        u8 func  = (inst >>  0) & MASK(6);

        printf("r%d, r%d, r%d, 0x%x, 0x%x\n", rs, rt, rd, shamt, func);
        instrs[op].f.r_type(rs, rt, rd, shamt, func);
        break;
    }
    }

    cpu.pc += 4;
}

