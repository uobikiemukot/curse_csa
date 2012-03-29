include ../makefile.def

PROG += manual nobrain

all: $(PROG)

manual: manual.c $(HDR)
	gcc $(CFLAGS) $(OBJ) manual.c -o manual

nobrain: nobrain.c $(HDR)
	gcc $(CFLAGS) $(OBJ) nobrain.c -o nobrain
