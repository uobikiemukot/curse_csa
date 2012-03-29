void search_moti(state *board, ktab *khead, int player,
	bitboard_t *rev, int to_bbindex)
{
	int type = bbindex2type(to_bbindex);
	bitboard_t tmp;
	kouho kh;

	init_kouho(&kh);
	kh.to_bbindex = to_bbindex;

	bit_copy(&tmp, rev);

	while (bit_test(&tmp)) {
		kh.to = bit_first(&tmp);
		pos_xor(kh.to, &tmp);

		switch (type) {
		case FU:
			if (is_nihu(&kh, board) || is_hunari(&kh))
				continue;
			break;
		case KE:
		case KY:
			if (is_hunari(&kh))
				continue;
			break;
		}

		add(&kh, khead);
	}
}

void search_ban(state *board, ktab *khead, int player,
	int from, int from_bbindex, bitboard_t *mask, bitboard_t *jmp, int oute_flag)
{
	int type = bbindex2type(from_bbindex);
	bitboard_t ret, tmp;
	kouho kh;

	init_kouho(&kh);
	kh.from = from;
	kh.from_bbindex = kh.to_bbindex = from_bbindex;

	bit_clear(&ret);
	kiki(&kh, &board->uni[OCC], &ret);
	if (oute_flag && type != OU)
		bit_and(&ret, mask);

	while (bit_test(&ret)) {
		kh.to = bit_first(&ret);
		pos_xor(kh.to, &ret);

		kh.captured_bbindex = -1;

		if (cannot_get(&kh, board, player))
			continue;

		if (type == OU) {
			if (attacked_by_normal(&kh, board, player))
				continue;
			if (attacked_by_jump(&kh, board, player))
				continue;
		}
		else {
			pos_and3(&tmp, kh.from, jmp);
			if (bit_test(&tmp))
				if (attacked_by_jump(&kh, board, player))
					continue;
		}

		if (can_nari(&kh)) {
			kh.to_bbindex += PROMOTE;
			add(&kh, khead);
			kh.to_bbindex -= PROMOTE;
		}

		switch (type) {
		case FU:
		case KE:
		case KY:
			if (is_hunari(&kh))
				continue;
		}

		add(&kh, khead);
	}
}

void search(state *board, ktab *khead, int player)
{
	int i, pos, oute_flag;
	bitboard_t rev, mask, tmp, jmp;

	bit_or3(&board->uni[OCC], &board->uni[0], &board->uni[1]);
	pos = (player) ? bit_first(&board->ban[G_OU]): bit_first(&board->ban[S_OU]);
	kiki_jmp(pos, &board->uni[OCC], &jmp);

	/* oute check */
	oute_flag = oute_check(board, player, &mask);
	if (oute_flag == OUTE_BY_NORMAL)
		goto ban;

	/* from moti */
	bit_not(&rev, &board->uni[OCC]);
	if (oute_flag == OUTE_BY_JUMP)
		bit_and(&rev, &mask);

	for (i = S_KI + player; i <= G_KA; i += 2) {
		if (board->moti[i])
			search_moti(board, khead, player, &rev, i);
	}

	/* from ban */
ban:
	for (i = player; i < B; i += 2) {
		bit_copy(&tmp, &board->ban[i]);
		while (bit_test(&tmp)) {
			pos = bit_first(&tmp);
			pos_xor(pos, &tmp);
			search_ban(board, khead, player, pos, i, &mask, &jmp, oute_flag);
		}
	}
}
