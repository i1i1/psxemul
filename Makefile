MK      := generated.mk
VARS    := variables.mk
HELPERS := helpers.mk

MAKE := make --no-print-directory

include $(HELPERS)

all: $(MK)
	@$(MAKE) -f $(MK) -j$(JOBS) dirs
	@$(MAKE) -f $(MK) -j$(JOBS) deps
	@$(MAKE) -f $(MK) -j$(JOBS)

-include $(VARS)

DEPS := $(addprefix build/, $(SRC:.c=.d))
OBJ  := $(addprefix build/, $(SRC:.c=.o))
DIRS := $(shell echo $(foreach o,$(OBJ),$(shell dirname $(o))) | tr ' ' '\n'|sort|uniq|xargs echo)

$(MK): $(VARS) Makefile $(HELPERS) .list
	@rm -f $(MK)
	$(call gener_file, $(MK), $(HELPERS), $(VARS))

.list:
	@if [ "$$(cat .list)" != "$(SRC) $(HDR)"  ]; then \
		echo "$(SRC) $(HDR)" >.list;\
	fi

fullclean: clean
	@rm -f $(MK)

clean:
	@rm -rf build/
	@rm -f $(OUT)

%:
	@$(MAKE) -f $(MK) -j$(JOBS) $(MAKECMDGOALS)


.PHONY: all fullclean .list

