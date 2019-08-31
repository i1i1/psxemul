#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "macro.h"
#include "types.h"
#include "utils.h"


void
print_bin(u64 num)
{
    int i;

    printf("0b");

    for (i = 63; i >= 0; i--) {
        if (num & BIT(i))
            break;
    }
    if (i < 0) {
        printf("0");
        return;
    }

    for (; i >= 0; i--)
        putchar((num & BIT(i)) ? '1' : '0');
}


void
die(char *fmt, ...)
{
    va_list args;

    fflush(stdout);

    va_start(args, fmt);

    fprintf(stderr, "\terr: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);

    exit(1);
}



