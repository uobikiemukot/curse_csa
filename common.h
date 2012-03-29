#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "SFMT/SFMT.h"

/* magic number */
enum { /* koma type */
	OU = 0, KI, FU, GI, KE,
	KY, HI, KA, TO, NG,
	NK, NY, RY, UM,
};

enum { /* index of bitboards */
	S_OU = 0, G_OU, S_KI, G_KI, S_FU, G_FU,
	S_GI, G_GI, S_KE, G_KE, S_KY, G_KY,
	S_HI, G_HI, S_KA, G_KA, S_TO, G_TO,
	S_NG, G_NG, S_NK, G_NK, S_NY, G_NY,
	S_RY, G_RY, S_UM, G_UM,
};

enum { /* ban of board */
	A1 = 0, B1, C1, D1, E1, F1, G1, H1, I1,
		A2, B2, C2, D2, E2, F2, G2, H2, I2,
		A3, B3, C3, D3, E3, F3, G3, H3, I3,
		A4, B4, C4, D4, E4, F4, G4, H4, I4,
		A5, B5, C5, D5, E5, F5, G5, H5, I5,
		A6, B6, C6, D6, E6, F6, G6, H6, I6,
		A7, B7, C7, D7, E7, F7, G7, H7, I7,
		A8, B8, C8, D8, E8, F8, G8, H8, I8,
		A9, B9, C9, D9, E9, F9, G9, H9, I9,
};

enum { /* common var */
	N = 9, /* board size: N x N */
	D = 3, /* division of bitboard */
	T = 14, /* types of koma: UM + 1 */
	M = 16, /* number of moti goma */
	B = 28, /* number of bitboards: T * 2 */
	SENTE = 0,
	GOTE = 1,
	OCC = 2,
	TRUE = 1,
	FALSE = 0,
	PROMOTE = 12,
	OUTE_BY_NORMAL = 1,
	OUTE_BY_JUMP = 2,
};

/* typedef */
typedef struct state state;
//typedef unsigned long long int hkey_t;
typedef struct kouho kouho; /* movement candidate */
typedef struct ktab ktab; /* header of dynamic array of kouho */
typedef struct node node;
//typedef struct khash kh_hash;
//typedef struct thash kt_hash;

struct kouho {
	int from, from_bbindex;
	int to, to_bbindex;
	int captured_bbindex;
	int count, sum, sqr, rate; // for uct
	double value;
	ktab *child;
};

struct ktab { /* array of kouho */
	int max; /* max element num */
	int num; /* current element num */
	int count;
	kouho *kh;
};

struct node {
	kouho *kh;
	node *next;
};

struct state {
	int moti[M]; /* S_OU, G_OU, S_KI, G_KI ... S_KA, G_KA: same order as bitboard index */

	bitboard_t ban[B];
	/* bitboard order:
		SENTE	GOTE
		 0: OU	3: OU
		 6: KI	...
		12: FU
		18: GI
		24: KE
		30: KY
		36: HI
		42: KA
		48: TO
		54: NG
		60: NK
		66: NY
		72: RY	...
		78: UM	81: UM

		data matrix: unsigned int * 3
		                       LSB
		 8  7  6  5  4  3  2  1  0
		17 16 15 14 13 12 11 10  9
		26 25 24 23 22 21 20 19 18

		35 34 33 32 31 30 29 28 27
		44 43 42 41 40 39 38 37 36
		53 52 51 50 49 48 47 46 45

		62 61 60 59 58 57 56 55 54
		71 70 69 68 67 66 65 64 63
		80 79 78 77 76 75 74 73 72
		MSB

		same data * 3
		27 * 3 = 81bit (unsigned int * 3 * 2 * 14)
	*/
	bitboard_t uni[3]; /* sente of occ, gote of occ, occupied bitboard */

	//hkey_t key; /* 64bit hash key */
};

/*
struct khash { // node info for uct
	//int rate; // select rate of random simulation
	//float sqr; // sum of square value
	//float value; // ucb1 or something
	int count; // sum of this node selected
	float sum; // sum of value
	kouho *kh;
	kh_hash *next;
};

struct thash { // hash of ktab
	ktab khead;
	hkey_t key;
	//kt_hash *next;
};
*/
