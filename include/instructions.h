#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <stdint.h>


struct instruction {
	char *nm;

	enum type {
		I_TYPE,
		J_TYPE,
		R_TYPE,
	} type;

	union {
		void (*j_type)(uint32_t target);
		void (*i_type)(uint8_t rs, uint8_t rt, uint16_t imm);
		void (*r_type)(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func);
	} f;
};

extern struct instruction instrs[64];


#endif /* _INSTRUCTIONS_H_ */
