#!/bin/sh
# [BUILD=builddir] ./run.sh FILES

: ${BUILD:="./build"}
: ${VARS:="./variables.mk"}

dirs_gen() {
	for dir in $@; do
		cat << EOF
#
# Making directory $dir'
#
$dir/.dir:
	@printf \$(PRINTF_FMT) MKDIR $dir
	@[ ! -d $dir ] && mkdir -p $dir
	@touch $dir/.dir

EOF
	done
}

objs_deps_gen() {
	for file in $srcs; do
		obj="$BUILD/$file.o"
		dep="$BUILD/$file.d"
		dir="$BUILD/$(dirname $file)"
		cat << EOF
#
# Making dependency file for $file
#
$dep: $file $dir/.dir
	@printf \$(PRINTF_FMT) DEP $dep
	@\$(CC) \$(CFLAGS) -MM $file | sed "s|^[^ ]*:|$obj:|" >$dep

#
# Making object file of $file
#
$obj: $file $dep $dir/.dir
	@printf \$(PRINTF_FMT) CC $obj
	@\$(CC) \$(CFLAGS) -c $file -o $obj

EOF
	done
}

morph_files() {
	ext=$1
	shift
	for item in $@; do
		printf "$BUILD/$item$ext\n"
	done | sort | uniq | tr '\n' ' '
}

uniq_dirs() {
	for item in $@; do
		printf "$BUILD/$(dirname $item)\n"
	done | sort | uniq
}

srcs=$(echo $@|tr ' ' '\n'|sort|uniq|tr '\n' ' ')
objs=$(morph_files .o $srcs)
deps=$(morph_files .d $srcs)
dirs=$(uniq_dirs $srcs)

cat <<EOF
include $VARS

OBJS += $objs
DEPS += $deps
DIRS += $dirs

PRINTF_FMT := "%-5s %s\n"

-include \$(DEPS)

all: \$(OUT)

#
# Linking target
#
\$(OUT): \$(OBJS)
	@printf \$(PRINTF_FMT) LD \$(OUT)
	@\$(CC) \$(CFLAGS) -o \$(OUT) \$(OBJS) \$(LDFLAGS)

########################
$(dirs_gen $dirs)
$(objs_deps_gen $@)
########################

.PHONY: all

EOF
