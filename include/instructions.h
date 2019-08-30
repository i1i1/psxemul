#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include "types.h"


struct instruction {
    char *nm;

    enum type {
        I_TYPE,
        J_TYPE,
        R_TYPE,
    } type;

    union {
        void (*j_type)(u32 target);
        void (*i_type)(u8 rs, u8 rt, u16 imm);
        void (*r_type)(u8 rs, u8 rt, u8 rd, u8 shamt, u8 func);
    } f;
};

extern struct instruction instrs[64];


#endif /* _INSTRUCTIONS_H_ */
