VARS  := variables.mk
GENER := ./gener.py
MK    := generated.mk

BUILD ?= ./build/

include $(HELPERS)

all: mk_list $(MK)
	@make -f $(MK) -j$(JOBS)

-include $(VARS)


$(MK): $(VARS) Makefile .list $(GENER)
	@$(GENER) --vars $(VARS) --build $(BUILD) -o $(MK) $(SRC)

mk_list:
	@if [ "$$(cat .list 2>/dev/null || echo -n)" != "$(SRC) $(HDR)"  ]; then \
		echo "$(SRC) $(HDR)" >.list;\
	fi

fullclean: clean
	@rm -f $(MK)
	@rm -f .list

clean: userclean
	@rm -rf build/
	@rm -f $(OUT)

%:
	@make -f $(MK) -j$(JOBS) $(MAKECMDGOALS)


.PHONY: all fullclean mk_list userclean

