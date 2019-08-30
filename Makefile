VARS  := variables.mk
GENER := ./gener.sh
MK    := generated.mk

BUILD ?= build


all: gener_mk
	@make -f $(MK) -j$(JOBS)

gener_mk: mk_list $(MK)

-include $(VARS)


$(MK): $(VARS) Makefile .list $(GENER)
	@printf "%-5s %s\n" REGEN $(MK)
	@BUILD=$(BUILD) VARS=$(VARS) $(GENER) $(SRC) > $(MK)

mk_list:
	@if [ "$$(cat .list 2>/dev/null || echo -n)" != "$(SRC) $(HDR)"  ]; then \
		echo "$(SRC) $(HDR)" >.list;\
	fi

fullclean: clean
	@rm -f $(MK)
	@rm -f .list

clean: userclean
	@rm -rf $(BUILD)
	@rm -f $(OUT)

%:
	@make -f $(MK) -j$(JOBS) $(MAKECMDGOALS)


.PHONY: all gener_mk fullclean mk_list userclean

