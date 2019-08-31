#ifndef _MACRO_H_
#define _MACRO_H_


#define STREQ(a, b)  (strcmp(a, b) == 0)
#define STRNEQ(a, b) (strcmp(a, b) != 0)
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))

#define CONFIG_SET(macro) _is_set_(macro)
#define _macrotest_1 ,
#define _is_set_(value) _is_set__(_macrotest_##value)
#define _is_set__(comma) _is_set___(comma 1, 0)
#define _is_set___(_, v, ...) v

#define BIT(n) (1ul << (n))
#define MASK(n) (BIT(n)-1ul)
#define MASK_BITS(f, t) (MASK(f-t) << t)
#define IS_ALIGNED(n, b) (!((n) & MASK(b)))
#define ROUND_DOWN(n, b) (((n) >> (b)) << (b))
#define ROUND_UP(n, b) (((((n) - 1ul) >> (b)) + 1ul) << (b))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define PASTE(a, b) a ## b
#define _STRINGIFY(a) #a
#define STRINGIFY(a) _STRINGIFY(a)

#include <string.h>

#endif /* _MACRO_H_ */
