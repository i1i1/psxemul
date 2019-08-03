define gener_dep =
	@$(CC) $(CFLAGS) -MM $(1) | sed "s|^[^ ]|build/`dirname $(1)`/&|" >$(2)
endef

define gener_vars =
	@echo "DEPS :=" $(DEPS) >>$(MK)
	@echo "OBJ  :=" $(OBJ)  >>$(MK)
	@echo "DIRS :=" $(DIRS) >>$(MK)
	@echo                   >>$(MK)
	@echo                   >>$(MK)
endef

define gener_dir =
	printf "%s:\n"           $(1)  >>$(MK);\
	printf "\t@mkdir -p %s\n" $(1) >>$(MK);\
	echo                           >>$(MK)
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
		$(call change_suf, $(1), .c, .d)      >>$(MK);\
	printf "\t@\$$(CC) \$$(CFLAGS) -c %s -o build/%s\n"\
		$(1) $(call change_suf, $(1), .c, .o) >>$(MK);\
	echo                                      >>$(MK);\
	printf "build/%s: %s\n"\
		$(call change_suf, $(1), .c, .d) $(1) >>$(MK);\
	printf "\t\$$(call gener_dep, \$$<, \$$@)">>$(MK);\
	echo                                      >>$(MK);\
	echo                                      >>$(MK)
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
	@echo -e "\t@echo MAKING DEPENDENCIES">>$(MK)
	@echo                             >>$(MK)
	@echo "dirs: \$$(DIRS)"           >>$(MK)
	@echo -e "\t@echo MAKING DIRECTORIES">>$(MK)
	@echo                             >>$(MK)
	@echo "\$$(OUT): \$$(OBJ)"        >>$(MK)
	@echo -e "\t@\$$(CC) \$$(CFLAGS) -o \$$@ \$$^ \$$(LDFLAGS)"\
							         >>$(MK)
	@echo                             >>$(MK)
endef

