define gener_dep =
	@printf "%-5s %s\n" DEP $(2)
	@$(CC) $(CFLAGS) -MM $(1) | sed "s|^[^ ]|build/`dirname $(1)`/&|" >$(2)
endef

define gener_cc =
	@printf "%-5s %s\n" CC $(2)
	@$(CC) $(CFLAGS) -c $(1) -o $(2)
endef

define gener_ld =
	@printf "%-5s %s\n" LD $(2)
	@$(CC) $(CFLAGS) -o $(2) $(1) $(LDFLAGS)
endef

define gener_vars =
	@echo "DEPS :=" $(DEPS) >>$(MK)
	@echo "OBJ  :=" $(OBJ)  >>$(MK)
	@echo "DIRS :=" $(DIRS) >>$(MK)
	@echo                   >>$(MK)
	@echo                   >>$(MK)
endef

define gener_dir =
	printf "%s:\n"              $(1) >>$(MK);\
	printf "\t@echo MKDIR %s\n" $(1) >>$(MK);\
	printf "\t@mkdir -p %s\n"   $(1) >>$(MK);\
	echo                             >>$(MK)
endef

define gener_dirs =
	@for i in $(DIRS); \
	do \
		$(call gener_dir, $$i); \
	done
endef

define change_suf =
	$$(printf "%s/%s%s" `dirname $(1)` `basename -s$(2) $(1)` $(3))
endef

define gener_src =
	printf "build/%s: build/%s\n"\
		$(call change_suf, $(1), .c, .o)\
		$(call change_suf, $(1), .c, .d)        >>$(MK);\
	printf "\t\$$(call gener_cc, $(1), build/%s)\n"\
		$(call change_suf, $(1), .c, .o)        >>$(MK);\
	echo                                        >>$(MK);\
	printf "build/%s: %s\n"\
		$(call change_suf, $(1), .c, .d) $(1)   >>$(MK);\
	printf "\t\$$(call gener_dep, \$$<, \$$@)\n">>$(MK);\
	echo                                        >>$(MK);\
	echo                                        >>$(MK)
endef

define gener_srcs =
	@for i in $(SRC); \
	do \
		$(call gener_src, $$i); \
	done
endef

define gener_out =
	@echo "all: \$$(DIRS) \$$(OUT)"   >>$(MK)
	@echo                             >>$(MK)
	@echo "deps: \$$(DEPS)"           >>$(MK)
	@echo -e "\t@true"                >>$(MK)
	@echo                             >>$(MK)
	@echo "dirs: \$$(DIRS)"           >>$(MK)
	@echo                             >>$(MK)
	@echo "\$$(OUT): \$$(OBJ)"        >>$(MK)
	@echo -e "\t\$$(call gener_ld, \$$^, \$$@)">>$(MK)
	@echo                             >>$(MK)
endef

define gener_file =
	@echo REGENERATING $(1)
	@echo "include"  $(2)            >> $(1)
	@echo "-include" $(3)            >> $(1)
	@echo                            >> $(1)
	$(call gener_vars)
	@echo                            >> $(1)
	$(call gener_out)
	@echo                            >> $(1)
	@echo "ifneq \"\$$(MAKECMDGOALS)\" \"dirs\"">> $(1)
	@echo "-include \$$(DEPS)"       >> $(1)
	@echo "endif"                    >> $(1)
	@echo                            >> $(1)
	$(call gener_dirs)
	@echo                            >> $(1)
	$(call gener_srcs)
	@echo                            >> $(1)
	@echo ".PHONY: all dirs deps"    >> $(1)
endef

