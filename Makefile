CC = gcc -std=c99
CFLAGS = -Wall -Wextra -ggdb -Iinclude -O3
LDFLAGS =

OUT = emul
BIOS = bios.bin

include gener.mk

test: all
	$(call prettify, RUN, $(OUT), \
	    ./$(OUT) $(BIOS))

.PHONY: test

