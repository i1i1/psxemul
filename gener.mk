Q = @
PREF := $(strip $(PREF))

# in order to preserve user variables
_SRC = $(SRC) $(wildcard src/*.c)
_HDR = $(HDR) $(wildcard include/*.h)
_OBJ = $(subst src, build, $(_SRC:.c=.o))
DEP =  $(subst src, build, $(_SRC:.c=.d))

all: $(PRE) $(DEP) $(_OBJ) $(OUT) gener.mk Makefile

define prettify =
	$(Q)for file in $(2) ; do printf "%5s\t%s%s\n" $(1) $(PREF) $$file ; done
	$(Q)$(3)
endef

ifneq ($(OUT),)
$(OUT): $(_OBJ) $(OBJ)
	$(call prettify, LD, $@, \
	    $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS))
endif

$(DEP): $(PRE)

build/%.d: src/%.c
	$(call prettify, DEP, $<, \
        $(CC) $(CFLAGS) -MM $< | sed -e "s!^[^ \t]\+\.o:!build/&!" > $@)

build/%.o: src/%.c build/%.d
	$(call prettify, CC, $<, \
	    $(CC) $(CFLAGS) -c $< -o $@)

clean: userclean
	$(call prettify, RM, $(_OBJ) $(OUT), \
	    rm -f $(_OBJ) $(OUT))

fullclean: clean
	$(call prettify, RM, $(DEP), \
	    rm -f $(DEP))

.PHONY: clean userclean fullclean all
.SECONDARY: $(DEP)

ifeq "$(MAKECMDGOALS)" "clean"
else
 ifeq "$(MAKECMDGOALS)" "fullclean"
 else
  -include $(DEP)
 endif
endif

