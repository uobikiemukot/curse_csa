void curse_debug()
{
	int p, q;
	char buf[CHARBUF];

	/* for debug */
	wmove(dump, 0, 0);
	wprintw(dump, "[debug info]\n");
	wprintw(dump, "y:%d x:%d (pos:%d) %s\n", y, x, pos,
		curse_sprintkoma(store_bbindex, buf));

	curse_pos2yx(pos, &q, &p);
	wprintw(dump, "pos2yx: pos:%d -> y:%d x:%d\n", pos, q, p);

	wprintw(dump, "\nplayer:%d khead.num:%d\n\n", player, khead.num);

	wprintw(dump, "store_select_y:%d store_select_x:%d\n", store_select_y, store_select_x);
	wprintw(dump, "store_moti_y:%d store_moti_x:%d\n", store_moti_y, store_moti_x);
	wprintw(dump, "store_pos:%d store_bbindex%d store_mode:%d\n", store_pos, store_bbindex, store_mode);

	wprintw(dump, "list_current_line:%d list_start_line:%d\nlist_lines:%d\n", list_current_line, list_start_line, list_lines);

	//wprintw(dump, "\n[mode] %s\n", mode_str[mode]);

	wrefresh(dump);
}

void curse_select_info()
{
	int i;
	kouho *kh;
	bitboard_t tmp;

	store_bbindex = -1;
	for (i = S_OU; i < B; i++) {
		pos_and3(&tmp, pos, &board->ban[i]);
		if (bit_test(&tmp)) {
			store_bbindex = i;
			break;
		}
	}

	bit_clear(&selected);
	bit_clear(&reverse);
	werase(list);
	list_lines = 0;

	for (i = 0; i < khead.num; i++) {
		kh = &khead.kh[i];
		if (pos == kh->from) {
			if (list_lines < LIST_LINES) {
				wmove(list, list_lines, 0);
				curse_printkouho(list, kh);
			}
			pos_or(kh->to, &reverse);
			list_lines++;
		}
	}
	wrefresh(list);
}

void curse_moti_info()
{
	int i;
	kouho *kh;

	if (player == SENTE)
		store_bbindex = (y - SUB2_TOP_Y) * 2;
	else
		store_bbindex = bbindex2reverse((y - SUB2_TOP_Y) * 2);

	bit_clear(&selected);
	bit_clear(&reverse);
	werase(list);
	list_lines = 0;

	for (i = 0; i < khead.num; i++) {
		kh = &khead.kh[i];
		if (kh->from == -1 && kh->to_bbindex == store_bbindex) {
			if (list_lines < LIST_LINES) {
				wmove(list, list_lines, 0);
				curse_printkouho(list, kh);
			}
			pos_or(kh->to, &reverse);
			list_lines++;
		}
	}
	wrefresh(list);
}

void curse_move_info()
{
	int i, count = 0, line;
	kouho *kh;

	werase(list);

	for (i = 0; i < khead.num; i++) {
		kh = &khead.kh[i];
		if ((store_mode == SELECT && kh->from == pos)
			|| (store_mode == MOTI && kh->from == -1 && kh->to_bbindex == store_bbindex)) {
			if (list_current_line == count) {
				wattron(list, A_REVERSE);
				store_kh = kh;
			}
			if (list_start_line <= count && count < (LIST_LINES + list_start_line)) {
				wmove(list, line, 0);
				curse_printkouho(list, kh);
				line++;
			}
			wattroff(list, A_REVERSE);
			count++;
		}
	}

	bit_clear(&selected);
	pos_or(store_kh->to, &selected);

	wrefresh(list);
}

void curse_setinfo()
{
	int i;
	bitboard_t tmp;

	getyx(stdscr, y, x);
	pos = curse_yx2pos(y, x);

	//player = turn % 2;

	if (mode == SELECT)
		curse_select_info();
	else if (mode == MOTI)
		curse_moti_info();
	else if (mode == MOVE)
		curse_move_info();

	if (CURSE_DEBUG)
		curse_debug();

	move(MODE_Y, MODE_X);
	clrtoeol();
	printw("[mode] %s", mode_str[mode]);

	move(y, x);
}
