CC = gcc -std=c99
CFLAGS = -Wall -Wextra -ggdb -Iinclude
LDFLAGS =

OUT = emul
BIOS = bios.bin

include gener.mk

test: all
	$(call prettify, RUN, $(OUT), \
	    ./$(OUT) $(BIOS))

.PHONY: test

