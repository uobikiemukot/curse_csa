include ../makefile.def

GCC = gcc
PROG = curse
HDR += curse_print.h  curse_move.h  curse_common.h  curse_info.h  curse_util.h curse_csa.h

all: $(PROG)

curse: curse.c $(HDR) $(OBJ)
	$(GCC) $(CFLAGS) $(OBJ) curse.c -o $@ -lcursesw
