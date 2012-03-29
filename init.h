void setline(int d, int i, int j, int ban[N][N])
{
	int x, y;

	switch (d) {
	case 0:
	case 3:
	case 5:
		x = -1;
		break;
	case 1:
	case 6:
		x = 0;
		break;
	default:
		x = 1;
	}

	switch (d) {
	case 0:
	case 1:
	case 2:
		y = -1;
		break;
	case 3:
	case 4:
		y = 0;
		break;
	default:
		y = 1;
	}

	i += x;
	j += y;
	while (0 <= i && i < N && 0 <= j && j < N) {
		ban[i][j] = 1;
		i += x;
		j += y;
	}
}

void kiki_common(int pos, int table[9], int board[N][N])
{
	int i = pos_to_index[pos][0], j = pos_to_index[pos][1], p, r, s,
		diff[9][2] = { {-1, -1}, {0, -1}, {1, -1},
			{-1, 0}, {0, 0}, {1, 0},
			{-1, 1}, {0, 1}, {1, 1} };

	for (p = 0; p < 9; p++) {
		r = i + diff[p][0];
		s = j + diff[p][1];
		if (table[p] == 1
			&& (0 <= r && r < N)
			&& (0 <= s && s < N))
			board[r][s] = 1;
	}
}

void kiki_ou(int player, int pos, int board[N][N])
{
	/*
		1 1 1
		1 * 1
		1 1 1
	*/
	int ou[9] = { 1, 1, 1, 1, 0, 1, 1, 1, 1 };
	kiki_common(pos, ou, board);
}

void kiki_ki(int player, int pos, int board[N][N])
{
	/*
		1 1 1
		1 * 1
		0 1 0
	*/
	int s_ki[9] = { 1, 1, 1, 1, 0, 1, 0, 1, 0 },
		g_ki[9] = { 0, 1, 0, 1, 0, 1, 1, 1, 1 }, *ki;

	ki = (player == SENTE) ? s_ki: g_ki;
	kiki_common(pos, ki, board);
}

void kiki_gi(int player, int pos, int board[N][N])
{
	/*
		1 1 1
		0 * 0
		1 0 1
	*/
	int s_gi[9] = { 1, 1, 1, 0, 0, 0, 1, 0, 1 },
		g_gi[9] = { 1, 0, 1, 0, 0, 0, 1, 1, 1 }, *gi;

	gi = (player == SENTE) ? s_gi: g_gi;
	kiki_common(pos, gi, board);
}

void kiki_ke(int player, int pos, int board[N][N])
{
	/*
		1 0 1
		0 0 0
		0 * 0
	*/
	int i = pos_to_index[pos][0], j = pos_to_index[pos][1],	r, s, t;

	r = (player == SENTE) ? j - 2 : j + 2;
	s = i - 1;
	t = i + 1;

	if ((0 <= r && r < N)
		&& (0 <= s && s < N))
		board[s][r] = 1;

	if ((0 <= r && r < N)
		&& (0 <= t && t < N))
		board[t][r] = 1;
}

void kiki_hu(int player, int pos, int board[N][N])
{
	/*
		0 1 0
		0 * 0
		0 0 0
	*/
	int s_hu[9] = { 0, 1, 0, 0, 0, 0, 0, 0, 0 },
		g_hu[9] = { 0, 0, 0, 0, 0, 0, 0, 1, 0 }, *hu;
	
	hu = (player == SENTE) ? s_hu: g_hu;
	kiki_common(pos, hu, board);
}

void kiki_hi(int pos, int board[N][N])
{
	/*
		0 1 0
		1 * 1
		0 1 0
	*/
	int hi[9] = { 0, 1, 0, 1, 0, 1, 0, 1, 0 };
	kiki_common(pos, hi, board);
}

void kiki_ka(int pos, int board[N][N])
{
	/*
		1 0 1
		0 * 0
		1 0 1
	*/
	int ka[9] = { 1, 0, 1, 0, 0, 0, 1, 0, 1 };
	kiki_common(pos, ka, board);
}

void pseudo_kiki(int bbindex, int pos, int board[N][N])
{
	int type = bbindex2type(bbindex), player = bbindex2player(bbindex);

	switch (type) {
	case OU:
	case RY: /* = OU + HI */
	case UM: /* = OU + KA */
		kiki_ou(player, pos, board);
		break;
	case KI:
	case TO:
	case NG:
	case NK:
	case NY:
		kiki_ki(player, pos, board);
		break;
	case FU:
	case KY: /* for suicide check */
		kiki_hu(player, pos, board);
		break;
	case GI:
		kiki_gi(player, pos, board);
		break;
	case KE:
		kiki_ke(player, pos, board);
		break;
	case HI: /* for suicide check */
		kiki_hi(pos, board);
		break;
	case KA: /* for suicide check */
		kiki_ka(pos, board);
		break;
	}
}

void unhash_rank(unsigned int i, int line[N])
{
	/*
		  6543210
		i:7654321
	*/
	line[0] = 0;
	line[1] = (i & 0x01) >> 0;
	line[2] = (i & 0x02) >> 1;
	line[3] = (i & 0x04) >> 2;
	line[4] = (i & 0x08) >> 3;
	line[5] = (i & 0x10) >> 4;
	line[6] = (i & 0x20) >> 5;
	line[7] = (i & 0x40) >> 6;
	line[8] = 0;
}

void unhash_file(unsigned int val, int line[N], int file)
{
	/*
		  76543210
		0:54321*76
		1:4321*765
		2:321*7654
		3:21*76543
		4:1*765432
		5:*7654321
		6:7654321*
		7:654321*7
		8:54321*76
	*/
	int list[8][8] = {
		{6, 7, -1, 1, 2, 3, 4, 5},
		{5, 6, 7, -1, 1, 2, 3, 4},
		{4, 5, 6, 7, -1, 1, 2, 3},
		{3, 4, 5, 6, 7, -1, 1, 2},
		{2, 3, 4, 5, 6, 7, -1, 1},
		{1, 2, 3, 4, 5, 6, 7, -1},
		{-1, 1, 2, 3, 4, 5, 6, 7},
		{7, -1, 1, 2, 3, 4, 5, 6},
	}, mod = file % 8, i;

	line[0] = line[8] = 0;

	for (i = 0; i < 8; i++)
		if (list[mod][i] != -1)
			line[list[mod][i]] = (val & (1 << i)) >> i;
}

unsigned int check(int pos, int line[N])
{
	int i;
	unsigned int ret = 0;

	i = pos + 1;
	while (0 <= i && i < N) {
		ret |= 1 << i;
		if (line[i] == 1)
			break;
		i++;
	}

	i = pos - 1;
	while (0 <= i && i < N) {
		ret |= 1 << i;
		if (line[i] == 1)
			break;
		i--;
	}
	
	return ret;
}

void set_rank(bitboard_t *tmp, int rank, unsigned int ret)
{
	bit_clear(tmp);

	if (rank < 3) {
		tmp->p[0] = ret << (rank * N);
		tmp->p[1] = 0;
		tmp->p[2] = 0;
	}
	else if (rank < 6) {
		tmp->p[0] = 0;
		tmp->p[1] = ret << (rank - 3) * N;
		tmp->p[2] = 0;
	}
	else {
		tmp->p[0] = 0;
		tmp->p[1] = 0;
		tmp->p[2] = ret << (rank - 6) * N;
	}
}

void set_file(bitboard_t *tmp, int file, unsigned int ret)
{
	int i;

	bit_clear(tmp);

	for (i = 0; i < N; i++) {
		if (ret & (1 << i))
			pos_xor(i * N + file, tmp);
	}
}

void expand(bitboard_t *bit, int pos, int flag)
{
	int i, j, count, rank = pos / N, file = pos % N;
	bitboard_t tmp;

	i = pos + 1;
	count = 1;
	while ((i / N) == rank) {
		pos_and3(&tmp, i, bit);
		if (bit_test(&tmp)) {
			pos_xor(i, bit);
			j = (flag == 0) ? i - count * N: i + count * N;
			if (0 <= j && j < N * N)
				pos_xor(j, bit);
			count++;
			i++;
		}
		else
			break;
	}

	i = pos - 1;
	count = 1;
	while ((i / N) == rank) {
		pos_and3(&tmp, i, bit);
		if (bit_test(&tmp)) {
			pos_xor(i, bit);
			j = (flag == 0) ? i - count * N: i + count * N;
			if (0 <= j && j < N * N)
				pos_xor(j, bit);
			count++;
			i--;
		}
		else
			break;
	}
}

void makerank(int type, int flag, bitboard_t table[128][N * N])
{
	int pos, rank, file, line[N];
	unsigned val, ret;
	bitboard_t tmp;

	for (val = 0; val <= 0x7f; val++) {
		for (pos = 0; pos < N * N; pos++) {
			rank = pos / N;
			file = pos % N;
			unhash_rank(val, line);

			ret = check(file, line);
			set_rank(&tmp, rank, ret);
			if (type == KA)
				expand(&tmp, pos, flag);

			bit_copy(&table[val][pos], &tmp);
		}
	}
}

void makefile(int type, int flag, bitboard_t table[256][N * N])
{
	int pos, rank, file, line[N];
	unsigned int val, ret;
	bitboard_t tmp;

	for (val = 0; val <= 0xff; val++) {
		for (pos = 0; pos < N * N; pos++) {
			rank = pos / N;
			file = pos % N;
			unhash_file(val, line, file);

			ret = check(rank, line);
			set_file(&tmp, file, ret);
			if (type == KY) {
				if (flag == 0)
					bit_and3(&table[val][pos], &tmp, &line_table[1][pos]);
				else
					bit_and3(&table[val][pos], &tmp, &line_table[6][pos]);
			}
			else
				bit_copy(&table[val][pos], &tmp);
		}
	}
}

void init_data()
{
	int i, j, k, l, p, q, d1, d2,
		rank, file, pos, index, type, ban[N][N], tmp[D];

	/* pos to bit */
	for	(i = 0; i < N * N; i++) {
		if (i < 27)
			bit_set(&pos_to_bit[i], 1 << i, 0, 0);
		else if (i < 54)
			bit_set(&pos_to_bit[i], 0, 1 << (i - 27), 0);
		else
			bit_set(&pos_to_bit[i], 0, 0, 1 << (i - 54));
	}

	/* pos to index */
	for (i = 0; i < N * N; i++) {
		rank = i / N;
		file = i % N;
		pos_to_index[i][0] = file;
		pos_to_index[i][1] = rank;
	}

	/* random table */
	/*
	init_gen_rand(1234);
	for (i = 0; i < B; i++) {
		for (j = 0; j < N * N + 19; j++) {
			random_table[i][j] = gen_rand64();
		}
	}
	*/

	/* rank table */
	rank = 0;
	for (i = 0; i < N; i++)
		rank |= 1 << i;

	for (i = 0; i < N; i++) {
		if (i < 3)
			bit_set(&rank_table[i], rank << i * N, 0, 0);
		else if (i < 6)
			bit_set(&rank_table[i], 0, rank << ((i - 3) * N), 0);
		else
			bit_set(&rank_table[i], 0, 0, rank << ((i - 6)* N));
	}

	/* file table */
	for (i = 0; i < N; i++) {
		file = 0;
		file |= 1 << i;
		file |= 1 << (i + N);
		file |= 1 << (i + N * 2);
		bit_set(&file_table[i], file, file, file);
	}

	/* line table */
	for (i = 0; i < 8; i++) {
		for (j = 0; j < N * N; j++) {
			memset(ban, 0, sizeof(ban));
			memset(tmp, 0, sizeof(tmp));
			setline(i, pos_to_index[j][0], pos_to_index[j][1], ban);
			for (k = 0; k < N; k++) {
				for (l = 0; l < N; l++) {
					if (ban[k][l] == 1) {
						pos = k + l * N;
						if (pos < 27)
							tmp[0] |= 1 << pos;
						else if (pos < 54)
							tmp[1] |= 1 << (pos - 27);
						else
							tmp[2] |= 1 << (pos - 54);
					}
				}
			}
			bit_set(&line_table[i][j], tmp[0], tmp[1], tmp[2]);
		}
	}

	/* jump table */
	for (index = 0; index < B; index++) { // from index
		type = bbindex2type(index);
		for (p = 0; p < N * N; p++) { // from p
			pos2index(p, &i, &j);
			for (q = 0; q < N * N; q++) { // to q
				if (p == q || (type != KY && type != HI && type != KA && type != RY && type != UM)) {
					bit_clear(&jump_table[index][p][q]);
					continue;
				}

				pos2index(q, &k, &l);
				if (i == k &&
					(((index == S_KY && p < q) || (index == G_KY && p > q))
					|| (type == HI || type == RY))) {
					d1 = (p > q) ? 1 : 6;
					d2 = (p > q) ? 6 : 1;
					bit_and3(&jump_table[index][p][q], &line_table[d1][p], &line_table[d2][q]);
				}
				else if (j == l && (type == HI || type == RY)) {
					d1 = (p > q) ? 3 : 4;
					d2 = (p > q) ? 4 : 3;
					bit_and3(&jump_table[index][p][q], &line_table[d1][p], &line_table[d2][q]);
				}
				else if ((abs(i - k) == abs(j - l)) && (type == KA || type == UM)) {
					d1 = ((i > k) && (j > l)) ? 0:
						((i > k) && (j < l)) ? 5:
						((i < k) && (j < l)) ? 7:
						2;
					d2 = ((i > k) && (j > l)) ? 7:
						((i > k) && (j < l)) ? 2:
						((i < k) && (j < l)) ? 0:
						5;
					bit_and3(&jump_table[index][p][q], &line_table[d1][p], &line_table[d2][q]);
				}
				else
					bit_clear(&jump_table[index][p][q]);
			}
		}
	}

	/* kiki table */
	for (i = 0; i < B; i++) {
		for (j = 0; j < N * N; j++) {
			memset(ban, 0, sizeof(ban));
			memset(tmp, 0, sizeof(tmp));
			pseudo_kiki(i, j, ban);
			for (k = 0; k < N; k++) {
				for (l = 0; l < N; l ++) {
					if (ban[k][l] == 1) {
						pos = k + l * N;
						tmp[0] |= pos_to_bit[pos].p[0];
						tmp[1] |= pos_to_bit[pos].p[1];
						tmp[2] |= pos_to_bit[pos].p[2];
					}
				}
			}
			bit_set(&kiki_table[i][j], tmp[0], tmp[1], tmp[2]);
		}
	}

	/* mask table for kindergarten bitboards */
	for (i = 0; i < N; i++)
		bit_and3(&file_mask_table[i], &line_table[1][N * 8 + i], &line_table[6][i]);

	/* rank mask table */
	for (i = 0; i < N; i++)
		bit_and3(&rank_mask_table[i], &line_table[3][i * N + 8], &line_table[4][i * N]);

	/* over mask table */
	for (i = 0; i < N * N; i++)
		bit_or3(&over_mask_table[i], &line_table[0][i], &line_table[2][i]);

	/* under mask table */
	for (i = 0; i < N * N; i++)
		bit_or3(&under_mask_table[i], &line_table[5][i], &line_table[7][i]);

	/* kindergarten bitboards */
	makerank(HI, 0, hi_rank_attack_table);
	makefile(HI, 0, hi_file_attack_table);
	makerank(KA, 0, ka_over_attack_table);
	makerank(KA, 1, ka_under_attack_table);
	makefile(KY, 0, ky_over_attack_table);
	makefile(KY, 1, ky_under_attack_table);
}

/* set random seed of SFMT */
void init_rand()
{
	int seed = time(NULL);
	init_gen_rand(seed);
}

void init_ban(state *board)
{
	int i, pos;
	bitboard_t tmp;

	/* init moti goma */
	for (i = 0; i < M; i++)
		board->moti[i] = 0;

	/* init bitboards */
	bit_set(&board->ban[S_OU], 0, 0, 0x400000);
	bit_set(&board->ban[G_OU], 0x10, 0, 0);
	bit_set(&board->ban[S_KI], 0, 0, 0xA00000);
	bit_set(&board->ban[G_KI], 0x28, 0, 0);
	bit_set(&board->ban[S_FU], 0, 0, 0x1FF);
	bit_set(&board->ban[G_FU], 0x7FC0000, 0, 0);
	bit_set(&board->ban[S_GI], 0, 0, 0x1100000);
	bit_set(&board->ban[G_GI], 0x44, 0, 0);
	bit_set(&board->ban[S_KE], 0, 0, 0x2080000);
	bit_set(&board->ban[G_KE], 0x82, 0, 0);
	bit_set(&board->ban[S_KY], 0, 0, 0x4040000);
	bit_set(&board->ban[G_KY], 0x101, 0, 0);
	bit_set(&board->ban[S_HI], 0, 0, 0x400);
	bit_set(&board->ban[G_HI], 0x10000, 0, 0);
	bit_set(&board->ban[S_KA], 0, 0, 0x10000);
	bit_set(&board->ban[G_KA], 0x400, 0, 0);

	for (i = S_TO; i <= G_UM; i++) /* nari goma */
		bit_clear(&board->ban[i]);

	bit_createall(board); /* create occupied bitboard */

	/* init hash key */
	/*
	board->key = 0;
	for (i = 0; i < B; i++) {
		bit_copy(&tmp, &board->ban[i]);
		while (bit_test(&tmp)) {
			pos = bit_first(&tmp);
			pos_xor(pos, &tmp);
			board->key ^= random_table[i][pos];
		}
	}
	for (i = 0; i < M; i++)
		board->key ^= random_table[i][N * N + board->moti[i]];
	*/
}

void init(state *board)
{
	init_data();
	init_rand();
	init_ban(board);
}
