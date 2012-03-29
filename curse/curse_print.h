enum {
	CHARBUF = 4,
};

void curse_sprintx68k(int index, char *buf)
{
	/*
		ㄅ ㄆ ㄇ ㄈ ㄉ ㄊ ㄋ ㄌ ㄍ ㄎ ㄏ ㄐ ㄑ ㄒ ㄓ
		玉 王 飛 龍 角 馬 金 銀 全 桂 圭 香 杏 歩 と
	*/
	if (index == S_OU && player == SENTE)
		strncpy(buf, "玉", 3);
	else if (index == S_OU && player == GOTE)
		strncpy(buf, "王", 3);
	else if (index == G_OU && player == SENTE)
		strncpy(buf, "ㄆ", 3);
	else if (index == G_OU && player == GOTE)
		strncpy(buf, "ㄅ", 3);
	else if (index == S_KI)
		strncpy(buf, "金", 3);
	else if (index == G_KI)
		strncpy(buf, "ㄋ", 3);
	else if (index == S_FU)
		strncpy(buf, "歩", 3);
	else if (index == G_FU)
		strncpy(buf, "ㄒ", 3);
	else if (index == S_GI)
		strncpy(buf, "銀", 3);
	else if (index == G_GI)
		strncpy(buf, "ㄌ", 3);
	else if (index == S_KE)
		strncpy(buf, "桂", 3);
	else if (index == G_KE)
		strncpy(buf, "ㄎ", 3);
	else if (index == S_KY)
		strncpy(buf, "香", 3);
	else if (index == G_KY)
		strncpy(buf, "ㄐ", 3);
	else if (index == S_HI)
		strncpy(buf, "飛", 3);
	else if (index == G_HI)
		strncpy(buf, "ㄇ", 3);
	else if (index == S_KA)
		strncpy(buf, "角", 3);
	else if (index == G_KA)
		strncpy(buf, "ㄉ", 3);
	else if (index == S_TO)
		strncpy(buf, "と", 3);
	else if (index == G_TO)
		strncpy(buf, "ㄓ", 3);
	else if (index == S_NG)
		strncpy(buf, "全", 3);
	else if (index == G_NG)
		strncpy(buf, "ㄍ", 3);
	else if (index == S_NK)
		strncpy(buf, "圭", 3);
	else if (index == G_NK)
		strncpy(buf, "ㄏ", 3);
	else if (index == S_NY)
		strncpy(buf, "杏", 3);
	else if (index == G_NY)
		strncpy(buf, "ㄑ", 3);
	else if (index == S_RY)
		strncpy(buf, "龍", 3);
	else if (index == G_RY)
		strncpy(buf, "ㄈ", 3);
	else if (index == S_UM)
		strncpy(buf, "馬", 3);
	else if (index == G_UM)
		strncpy(buf, "ㄊ", 3);
}

void curse_sprintmplus(int index, char *buf)
{
	// АБВГДЕЁЖЗИЙКЛМН
	if (index == S_OU && player == SENTE)
		strncpy(buf, "玉", 3);
	else if (index == S_OU && player == GOTE)
		strncpy(buf, "А", 3);
	else if (index == G_OU && player == SENTE)
		strncpy(buf, "А", 3);
	else if (index == G_OU && player == GOTE)
		strncpy(buf, "Б", 3);
	else if (index == S_KI)
		strncpy(buf, "金", 3);
	else if (index == G_KI)
		strncpy(buf, "Ё", 3);
	else if (index == S_FU)
		strncpy(buf, "歩", 3);
	else if (index == G_FU)
		strncpy(buf, "М", 3);
	else if (index == S_GI)
		strncpy(buf, "銀", 3);
	else if (index == G_GI)
		strncpy(buf, "Ж", 3);
	else if (index == S_KE)
		strncpy(buf, "桂", 3);
	else if (index == G_KE)
		strncpy(buf, "И", 3);
	else if (index == S_KY)
		strncpy(buf, "香", 3);
	else if (index == G_KY)
		strncpy(buf, "К", 3);
	else if (index == S_HI)
		strncpy(buf, "飛", 3);
	else if (index == G_HI)
		strncpy(buf, "В", 3);
	else if (index == S_KA)
		strncpy(buf, "角", 3);
	else if (index == G_KA)
		strncpy(buf, "Д", 3);
	else if (index == S_TO)
		strncpy(buf, "と", 3);
	else if (index == G_TO)
		strncpy(buf, "Н", 3);
	else if (index == S_NG)
		strncpy(buf, "全", 3);
	else if (index == G_NG)
		strncpy(buf, "З", 3);
	else if (index == S_NK)
		strncpy(buf, "圭", 3);
	else if (index == G_NK)
		strncpy(buf, "Й", 3);
	else if (index == S_NY)
		strncpy(buf, "杏", 3);
	else if (index == G_NY)
		strncpy(buf, "Л", 3);
	else if (index == S_RY)
		strncpy(buf, "龍", 3);
	else if (index == G_RY)
		strncpy(buf, "Г", 3);
	else if (index == S_UM)
		strncpy(buf, "馬", 3);
	else if (index == G_UM)
		strncpy(buf, "Е", 3);
}

char *curse_sprintkoma(int index, char *buf)
{
	memset(buf, '\0', CHARBUF);

	if (player == GOTE)
		index = bbindex2reverse(index);

	if (FONTSET == X68K)
		curse_sprintx68k(index, buf);
	else if (FONTSET == MPLUS)
		curse_sprintmplus(index, buf);

	return buf;
}

void curse_printmoti(WINDOW *win, int moti[M], int player)
{
	int i, k, num;
	char buf[CHARBUF];

	wmove(win, 0, 0);

	/*
	if (player == SENTE)
		wprintw(win, "先手:\n");
	else
		wprintw(win, "後手:\n");
	*/

	if (win == sub1) {
		for (i = S_KA + player; i >= 0; i -= 2)
			wprintw(win, "%s:%d\n", curse_sprintkoma(i, buf), moti[i]);
	}
	else if (win == sub2) {
		for (i = player; i <= G_KA; i += 2)
			wprintw(win, "%s:%d\n", curse_sprintkoma(i, buf), moti[i]);
	}

	wrefresh(win);
}

void curse_printban_default()
{
	int i, j, k, pos;
	char buf[CHARBUF];
	bitboard_t tmp, rev, sel;

	curse_printmoti(sub1, board->moti, GOTE);

	wmove(ban, 0, 0);
	wprintw(ban, "   ９ ８ ７ ６ ５ ４ ３ ２ １\n");
	//wprintw(ban, "  九 八 七 六 五 四 三 二 一\n");
	//wprintw(ban, "  I  H  G  F  E  D  C  B  A\n");
	wprintw(ban, "  +--+--+--+--+--+--+--+--+--+\n");
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			// 8 7 6 5 4 3 2 1 0
			pos = i * N - j;
			pos_and3(&rev, pos, &reverse);
			pos_and3(&sel, pos, &selected);

			if (j == 1)
				wprintw(ban, "  |");

			if (bit_test(&rev))
				wattron(ban, A_REVERSE);

			if (bit_test(&sel))
				wattron(ban, COLOR_PAIR(2));

			for (k = 0; k < B; k++) {
				pos_and3(&tmp, pos, &board->ban[k]);
				if (bit_test(&tmp)) {
					wprintw(ban, "%s", curse_sprintkoma(k, buf));
					wattroff(ban, A_REVERSE);
					wattroff(ban, COLOR_PAIR(2));
					wprintw(ban, "|");
					break;
				}
			}

			if (k == B) {
				wprintw(ban, "　");
				wattroff(ban, A_REVERSE);
				wattroff(ban, COLOR_PAIR(2));
				wprintw(ban, "|");
			}


			if (j == N)
				wprintw(ban, "%s", v_str[i - 1]);
		}
		wprintw(ban, "\n");
		wprintw(ban, "  +--+--+--+--+--+--+--+--+--+\n");
	}
	wrefresh(ban);

	curse_printmoti(sub2, board->moti, SENTE);
}

void curse_printban_rotate()
{
	int i, j, k, pos;
	char buf[CHARBUF];
	bitboard_t tmp, rev, sel;

	curse_printmoti(sub1, board->moti, SENTE);

	wmove(ban, 1, 0);
	wprintw(ban, "  +--+--+--+--+--+--+--+--+--+\n");
	for (i = N; i > 0; i--) {
		for (j = 0; j < N; j++) {
			// 72 73 74 75 76 77 78 79 80
			pos = (i - 1) * N + j;
			pos_and3(&rev, pos, &reverse);
			pos_and3(&sel, pos, &selected);

			if (j == 0)
				wprintw(ban, "%s|", v_str[i - 1]);

			if (bit_test(&rev))
				wattron(ban, A_REVERSE);

			if (bit_test(&sel))
				wattron(ban, COLOR_PAIR(2));

			for (k = 0; k < B; k++) {
				pos_and3(&tmp, pos, &board->ban[k]);
				if (bit_test(&tmp)) {
					wprintw(ban, "%s", curse_sprintkoma(k, buf));
					wattroff(ban, A_REVERSE);
					wattroff(ban, COLOR_PAIR(2));
					wprintw(ban, "|");
					break;
				}
			}

			if (k == B) {
				wprintw(ban, "　");
				wattroff(ban, A_REVERSE);
				wattroff(ban, COLOR_PAIR(2));
				wprintw(ban, "|");
			}
		}
		wprintw(ban, "\n");
		wprintw(ban, "  +--+--+--+--+--+--+--+--+--+\n");
	}
	wprintw(ban, "   １ ２ ３ ４ ５ ６ ７ ８ ９");
	//wprintw(ban, "   一 二 三 四 五 六 七 八 九");
	//wprintw(ban, "  A  B  C  D  E  F  G  H  I");
	wrefresh(ban);

	curse_printmoti(sub2, board->moti, GOTE);
}

void curse_printban()
{
	if (player == SENTE)
		curse_printban_default();
	else
		curse_printban_rotate();
}

void curse_printkouho(WINDOW *win, kouho *kh)
{
	int i, j, k, l, index, owner = bbindex2player(kh->to_bbindex);
	char buf[CHARBUF];

	pos2index(kh->to, &k, &l);

	if (owner == SENTE)
		wprintw(win, "▲");
	else
		wprintw(win, "△");

	if (player == SENTE)
		index = (owner == GOTE) ? bbindex2reverse(kh->to_bbindex): kh->to_bbindex;
	else
		index = (owner == SENTE) ? bbindex2reverse(kh->to_bbindex): kh->to_bbindex;

	wprintw(win, "%s%s %s",
		h_str[k], v_str[l],
		curse_sprintkoma(index, buf));
}
