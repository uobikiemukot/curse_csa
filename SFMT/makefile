OPTI = -finline-functions -fomit-frame-pointer -DNDEBUG \
-fno-strict-aliasing --param max-inline-insns-single=1800
FLAG = --param inline-unit-growth=500 \
--param large-function-growth=900 -std=c99
CFLAGS = -O3 -msse2 -pipe \
	-DHAVE_SSE2=1 -DMEXP=19937 -I./SFMT

CC = gcc
OBJ = SFMT.o

all: $(OBJ)

SFMT.o: SFMT.c
	gcc -c $(CFLAGS) $(FLAG) $(OPTI) SFMT.c -o SFMT.o

clean:
	rm -f $(OBJ)
