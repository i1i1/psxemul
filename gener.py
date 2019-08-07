#!/usr/bin/python3

import sys, argparse

vars_template = '''
OBJS := %(objs)s
DEPS := %(deps)s
DIRS := %(dirs)s

PRINTF_FMT := "%%-5s %%s\\n"

#
# Default target. Makefile relies it to be the first one!
#
all:
'''

include_template = '''
include %(vars)s
-include $(DEPS)
'''

out_template = '''
all: $(OUT)

.PHONY: all

#
# Linking target
#
$(OUT): $(OBJS)
	@printf $(PRINTF_FMT) LD $(OUT)
	@$(CC) $(CFLAGS) -o $(OUT) $(OBJS) $(LDFLAGS)
'''

obj_template = '''
#
# Making object file of `%(src)s'
#
%(obj)s: %(src)s %(dep)s %(dir)s/.dir
	@printf $(PRINTF_FMT) CC %(obj)s
	@$(CC) $(CFLAGS) -c %(src)s -o %(obj)s
'''

dep_template = '''
#
# Making dependency file for `%(src)s'
#
%(dep)s: %(src)s %(dir)s/.dir
	@printf $(PRINTF_FMT) DEP %(dep)s
	@$(CC) $(CFLAGS) -MM %(src)s | sed "s|^[^ ]*:|%(obj)s:|" >%(dep)s
'''

dir_template = '''
#
# Making directory `%(dir)s'
#
%(dir)s/.dir:
	@printf $(PRINTF_FMT) MKDIR %(dir)s
	@[ ! -d %(dir)s ] && mkdir -p %(dir)s
	@touch %(dir)s/.dir
'''


def get_dir(s):
    return '/'.join(s.split('/')[:-1])

def list_to_str(l):
    s = ""
    for i in l:
        s = s + " " + str(i)
    return s


def gener_includes(fp, vars):
    print(include_template % {
        "vars" : vars,
    }, file=fp)


def gener_out(fp):
    print(out_template, file=fp)


def gener_dirs(fp, dirs):
    for d in dirs:
        print(dir_template % {
            "dir" : d,
        }, file=fp)


def gener_objs(fp, build, srcs):
    for s in srcs:
        print(obj_template % {
            "src" : s,
            "obj" : build + s + ".o",
            "dep" : build + s + ".d",
            "dir" : build + get_dir(s),
        }, file=fp)


def gener_deps(fp, build, srcs):
    for s in srcs:
        print(dep_template % {
            "src" : s,
            "obj" : build + s + ".o",
            "dep" : build + s + ".d",
            "dir" : build + get_dir(s),
        }, file=fp)


def gener_vars(fp, deps, objs, dirs):
    print(vars_template % {
        "objs" : list_to_str(objs),
        "deps" : list_to_str(deps),
        "dirs" : list_to_str(dirs),
    }, file=fp)


def main(fp, build, vars, srcs):
    deps = set(map(lambda s : build + s + ".d", srcs.copy()))
    objs = set(map(lambda s : build + s + ".o", srcs.copy()))
    dirs = set(map(lambda s : build + get_dir(s), srcs.copy()))

    gener_vars(fp, deps, objs, dirs)
    gener_includes(fp, vars)
    gener_dirs(fp, dirs)
    gener_objs(fp, build, srcs)
    gener_deps(fp, build, srcs)
    gener_out(fp)



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-v", "--vars", type=str, default="variables.mk",
                        help="variables file")
    parser.add_argument("-b", "--build", type=str, default="./build/",
                        help="Build directory")
    parser.add_argument("-o", "--out", type=argparse.FileType('w'), required=True,
                        help="output file")
    parser.add_argument('files', metavar='FILE', type=str, nargs='*',
                        help='files for dependencies')
    args = parser.parse_args()
    main(args.out, args.build, args.vars, set(args.files))

