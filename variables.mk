CC := gcc -std=c99
CFLAGS := -Wall -Wextra -ggdb -Iinclude -O3

SRC := $(shell find src -name "*.c")
HDR := $(shell find include -name "*.h")
OUT := emul

JOBS:=8

BIOS:=bios.bin

all:

test: all
	./$(OUT) $(BIOS)

$(SRC): untabify

untabify:
	@echo UNTABIFY
	@awk '/\t/ {printf("%s:%d:%s\n", FILENAME, FNR, $$0); a=1} END{exit(a)}' $(SRC) $(HDR)

.PHONY: untabify

