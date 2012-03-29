/*
	http://www-als.ics.nitech.ac.jp/paper/H19-S/oso.pdf
*/
int attacked_by_jump(kouho *kh, state *board, int player)
{
	int from, to, your, my, type;
	bitboard_t tmp, jmp, occ;

	make_move(kh, board);
	bit_or3(&occ, &board->uni[0], &board->uni[1]);

	from = (player) ? bit_first(&board->ban[G_OU]): bit_first(&board->ban[S_OU]);

	for (my = player; my < B; my += 2) {
		type = bbindex2type(my);
		your = bbindex2reverse(my);

		if (type == KY || type == HI || type == KA
			|| type == RY || type == UM) {
			bit_copy(&tmp, &board->ban[your]);
			while (bit_test(&tmp)) {
				to = bit_first(&tmp);
				pos_xor(to, &tmp);

				if (bit_zero(&jump_table[your][from][to]))
					continue;

				bit_and3(&jmp, &occ, &jump_table[your][from][to]);
				if (bit_zero(&jmp)) { /* attacked by jump koma */
					unmake_move(kh, board);
					return 1;
				}
			}
		}
	}

	/* not attacked by any koma */
	unmake_move(kh, board);
	return 0;
}

int attacked_by_normal(kouho *kh, state *board, int player)
{
	int from, my, your;
	bitboard_t tmp;

	make_move(kh, board);

	from = (player) ? bit_first(&board->ban[G_OU]): bit_first(&board->ban[S_OU]);

	for (my = player; my < B; my += 2) {
		your = bbindex2reverse(my);

		bit_and3(&tmp, &kiki_table[my][from], &board->ban[your]);
		if (bit_test(&tmp)) { /* killed by normal koma */
			unmake_move(kh, board);
			return 1;
		}
	}

	/* not killed by any koma */
	unmake_move(kh, board);
	return 0;
}

int oute_check(state *board, int player, bitboard_t *mask)
{
	int from, to, your, my, type;
	bitboard_t tmp;

	from = (player) ? bit_first(&board->ban[G_OU]): bit_first(&board->ban[S_OU]);

	for (my = player; my < B; my += 2) {
		type = bbindex2type(my);
		your = bbindex2reverse(my);

		if (type == KY || type == HI || type == KA
			|| type == RY || type == UM) {
			bit_and3(mask, &kiki_table[my][from], &board->ban[your]);
			if (bit_test(mask)) { /* attacked by jump koma (but not jump attack) */
				return OUTE_BY_NORMAL;
			}

			bit_copy(&tmp, &board->ban[your]);
			while (bit_test(&tmp)) {
				to = bit_first(&tmp);
				pos_xor(to, &tmp);

				if (bit_zero(&jump_table[your][from][to]))
					continue;

				bit_and3(mask, &board->uni[OCC], &jump_table[your][from][to]);
				if (bit_zero(mask)) { /* attacked by jump koma */
					pos_or3(mask, to, &jump_table[your][from][to]);
					return OUTE_BY_JUMP;
				}
			}
		}
		else {
			bit_and3(mask, &kiki_table[my][from], &board->ban[your]);
			if (bit_test(mask)) { /* attacked by normal koma */
				return OUTE_BY_NORMAL;
			}
		}
	}

	/* not attacked by any koma */
	return 0;
}

int is_nihu(kouho *kh, state *board)
{
	int file = kh->to % N;
	bitboard_t tmp;

	bit_and3(&tmp, &file_table[file], &board->ban[kh->to_bbindex]);
	if (bit_test(&tmp))
		return 1;

	return 0;
}

int cannot_get(kouho *kh, state *board, int player)
{
	int i, against = 1 - player;
	bitboard_t tmp;

	pos_and3(&tmp, kh->to, &board->uni[player]);
	if (bit_test(&tmp))
		return 1;

	pos_and3(&tmp, kh->to, &board->uni[against]);
	if (bit_test(&tmp)) {
		for (i = against; i < B; i += 2) {
			pos_and3(&tmp, kh->to, &board->ban[i]);
			if (bit_test(&tmp)) {
				kh->captured_bbindex = i;
				return 0;
			}
		}
	}

	return 0;
}

int can_nari(kouho *kh)
{
	int type = bbindex2type(kh->from_bbindex), player = bbindex2player(kh->from_bbindex);

	if (type >= TO || type == KI || type == OU)
		return 0;

	if (player == SENTE) {
		if (kh->from < 27 || kh->to < 27)
			return 1;
	}
	else {
		if (kh->from > 53 || kh->to > 53)
			return 1;
	}

	return 0;
}

int is_hunari(kouho *kh)
{
	int index = kh->to_bbindex;

	if (index == S_FU || index == S_KY) {
		if (kh->to < 9)
			return 1;
	}
	else if (index == G_FU || index == G_KY) {
		if (kh->to > 71)
			return 1;
	}
	else if (index == S_KE) {
		if (kh->to < 18)
			return 1;
	}
	else if(index == G_KE) {
		if (kh->to > 62)
			return 1;
	}

	return 0;
}
