void unmake_move(kouho *kh, state *board)
{
	int captured = kh->captured_bbindex, player = bbindex2player(kh->to_bbindex), moti;
	bitboard_t *ban = board->ban, *uni = board->uni;

	/* remove from dest pos */
	pos_xor(kh->to, &ban[kh->to_bbindex]);
	pos_xor(kh->to, &uni[player]);
	//board->key ^= random_table[kh->to_bbindex][kh->to];

	if (kh->from == -1) { /* from moti goma */
		//board->key ^= random_table[kh->to_bbindex][N * N + board->moti[kh->to_bbindex]];
		board->moti[kh->to_bbindex]++;
		//board->key ^= random_table[kh->to_bbindex][N * N + board->moti[kh->to_bbindex]];
	}
	else { /* from ban */
		/* go back to previous pos */
		pos_xor(kh->from, &ban[kh->from_bbindex]);
		pos_xor(kh->from, &uni[player]);
		//board->key ^= random_table[kh->from_bbindex][kh->from];

		if (captured != -1) { /* get koma on next pos */
			pos_xor(kh->to, &ban[captured]);
			pos_xor(kh->to, &uni[1 - player]);
			//board->key ^= random_table[captured][kh->to];

			moti = bbindex2moti(captured);
			//board->key ^= random_table[moti][N * N + board->moti[moti]];
			board->moti[moti]--;
			//board->key ^= random_table[moti][N * N + board->moti[moti]];
		}
	}
}

void make_move(kouho *kh, state *board)
{
	int captured = kh->captured_bbindex, player = bbindex2player(kh->to_bbindex), moti;
	bitboard_t *ban = board->ban, *uni = board->uni;

	if (kh->from == -1) { /* from moti goma */
		//board->key ^= random_table[kh->to_bbindex][N * N + board->moti[kh->to_bbindex]];
		board->moti[kh->to_bbindex]--;
		//board->key ^= random_table[kh->to_bbindex][N * N + board->moti[kh->to_bbindex]];
	}
	else { /* from ban */
		/* remove koma on previous pos */
		pos_xor(kh->from, &ban[kh->from_bbindex]);
		pos_xor(kh->from, &uni[player]);
		//board->key ^= random_table[kh->from_bbindex][kh->from];

		if (captured != -1) { /* get koma on next pos */
			pos_xor(kh->to, &ban[captured]);
			pos_xor(kh->to, &uni[1 - player]);
			//board->key ^= random_table[captured][kh->to];

			moti = bbindex2moti(captured);
			//board->key ^= random_table[moti][N * N + board->moti[moti]];
			board->moti[moti]++;
			//board->key ^= random_table[moti][N * N + board->moti[moti]];
		}
	}

	/* move to dest pos */
	pos_xor(kh->to, &ban[kh->to_bbindex]);
	pos_or(kh->to, &uni[player]);
	//board->key ^= random_table[kh->to_bbindex][kh->to];
}
