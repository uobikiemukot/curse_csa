#include "../def.h"
#include <locale.h>
#include <signal.h>
#include <curses.h>

enum {
	/* position */
	LTOP_Y = 3,
	LTOP_X = 10,
	RTTOP_Y = 3,
	RTOP_X = 34,
	LBOTTOM_Y = 19,
	LBOTTOM_X = 10,
	RBOTTOM_Y = 19,
	RBOTTOM_X = 34,
	MOTI_Y = 13,
	MOTI_X = 40,
	INC_X = 3,
	INC_Y = 2,
	INC_SUB = 1,
	SUB1_TOP_Y = 4,
	SUB1_BOTTOM_Y = 11,
	SUB2_TOP_Y = 13,
	SUB2_BOTTOM_Y = 20,
	/* window position */
	SUB1_Y = 2,
	SUB1_X = 1,
	SUB2_Y = 13,
	SUB2_X = 40,
	BAN_Y = 1,
	BAN_X = 7,
	LIST_Y = 1,
	LIST_X = 47,
	SRV_Y = 11,
	SRV_X = 47,
	DUMP_Y = 1,
	DUMP_X = 93,
	MODE_Y = 23,
	MODE_X = 1,
	SENTE_Y = 21,
	SENTE_X = 47,
	/* window size */
	SUB_LINES = 8,
	SUB_COLS = 5,
	BAN_LINES = 21,
	BAN_COLS = 33,
	LIST_LINES = 9,
	LIST_COLS = 45,
	SRV_LINES = 10,
	SRV_COLS = 45,
	DUMP_LINES = 19,
	DUMP_COLS = 45,
	/* mode */
	SELECT = 0,
	MOVE = 1,
	INSERT = 2,
	MOTI = 3,
	/* key */
	KEY_RET = 10,
	KEY_ESC = 27,
	/* misc */
	X68K = 0,
	MPLUS = 1,
	FONTSET = 1, /* 0:x68k 1:mplus */
	CURSE_DEBUG = 0,
};

/* global */
int x, y, pos; /* cursor position */
int store_select_x = -1, store_select_y = -1; /* stored pos on SELECT mode */
int store_moti_x = -1, store_moti_y = -1; /* stored pos on MOTI mode */
int store_pos = -1, store_bbindex = -1, store_mode = -1;
int list_current_line = 0, list_start_line = 0, list_lines = 0;
kouho *store_kh = NULL;
int mode = SELECT;/* current mode */
//int turn = 0;
int player = SENTE;
char *mode_str[] = {"SELECT", "MOVE", "INSERT", "MOTI"};
/*
char *mode_str[] = {"SELECT\nmove:hjkl select:m change:t quit:q",
	"MOVE\nnext:n submit:r escape:e quit:q",
	"INSERT\n",
	"MOTI\nmove:jk select:m change:t quit:q"};
*/
char *v_str[] = {"一", "二", "三", "四", "五", "六", "七", "八", "九"};
char *h_str[] = {"１", "２", "３", "４", "５", "６", "７", "８", "９"};
WINDOW *ban, *sub1, *sub2, *srv, *list, *dump;

state *board;
ktab khead;
bitboard_t reverse, selected;/* reverse drawing flag */
