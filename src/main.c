#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#include "cpu.h"


char *help_message =
	"%s [bios] [game]\n";

void
help(char *cmd)
{
	printf(help_message, cmd);
}

int
main(int argc, char **argv)
{
	if (argc < 2) {
		help(argv[0]);
		return 1;
	}

	mem_init();
	cpu_init();

	load_bios(argv[1], &mem);

	for (;;) {
		cpu_iter();
	}

	return 0;
}
