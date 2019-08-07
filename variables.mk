CC := gcc -std=c99
CFLAGS := -Wall -Wextra -ggdb -Iinclude -O3

SRC := $(shell find src -name "*.c")
HDR := $(shell find include -name "*.h")
OUT := emul

JOBS:=8

BIOS:=bios.bin

test: all
	./$(OUT) $(BIOS)

