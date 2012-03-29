unsigned int rank_hash(bitboard_t *occ)
{
	unsigned int hash;

	/* rank attack hash for HI(yoko), KA */
	hash = occ->p[0] | occ->p[1] | occ->p[2];
	hash *= 0x40201;
	hash = (hash >> 19) & 0x7F;

	return hash;
}

unsigned int file_hash(bitboard_t *occ)
{
	unsigned int hash;

	/* file attack hash for HI(tate), KY */
	hash = (occ->p[0] >> 3) | occ->p[1] | (occ->p[2] << 3);
	hash *= 0x1010101;
 	hash = (hash >> 19) & 0xff;

	return hash;
}

void kiki(kouho *kh, bitboard_t *occ, bitboard_t *ret)
{
	int pos = kh->from, index = kh->from_bbindex,
		type = bbindex2type(kh->from_bbindex), player = bbindex2player(kh->from_bbindex),
		file = kh->from % N, rank = kh->from / N;
	unsigned int hash;
	bitboard_t tmp;

	/* Kindergarten Bitboards
		http://d.hatena.ne.jp/minute_hand/20110819/1313759957
		http://d.hatena.ne.jp/mclh46/20110820/1313797982
	*/

	switch (type) {
	case RY:
	case UM:
		bit_or(ret, &kiki_table[index][pos]);
	}

	switch (type) {
	case KY:
		bit_and3(&tmp, occ, &file_mask_table[file]);
		hash = file_hash(&tmp); // tate
		if (player)
			bit_or(ret, &ky_under_attack_table[hash][pos]);
		else
			bit_or(ret, &ky_over_attack_table[hash][pos]);
		break;
	case HI:
	case RY:
		bit_and3(&tmp, occ, &file_mask_table[file]);
		hash = file_hash(&tmp); // tate
		bit_or(ret, &hi_file_attack_table[hash][pos]);

		bit_and3(&tmp, occ, &rank_mask_table[rank]);
		hash = rank_hash(&tmp); // yoko
		bit_or(ret, &hi_rank_attack_table[hash][pos]);
		break;
	case KA:
	case UM:
		bit_and3(&tmp, occ, &over_mask_table[pos]);
		hash = rank_hash(&tmp); // over
		bit_or(ret, &ka_over_attack_table[hash][pos]);

		bit_and3(&tmp, occ, &under_mask_table[pos]);
		hash = rank_hash(&tmp); // under
		bit_or(ret, &ka_under_attack_table[hash][pos]);
		break;
	default:
		bit_or(ret, &kiki_table[index][pos]);
	}
}

void kiki_jmp(int pos, bitboard_t *occ, bitboard_t *ret) /* for jump check */
{
	int file, rank;
	unsigned int hash;
	bitboard_t tmp;

	file = pos % N;
	rank = pos / N;

	bit_and3(&tmp, occ, &file_mask_table[file]);
	hash = file_hash(&tmp); // tate
	bit_or(ret, &hi_file_attack_table[hash][pos]);

	bit_and3(&tmp, occ, &rank_mask_table[rank]);
	hash = rank_hash(&tmp); // yoko
	bit_or(ret, &hi_rank_attack_table[hash][pos]);

	bit_and3(&tmp, occ, &over_mask_table[pos]);
	hash = rank_hash(&tmp); // over
	bit_or(ret, &ka_over_attack_table[hash][pos]);

	bit_and3(&tmp, occ, &under_mask_table[pos]);
	hash = rank_hash(&tmp); // under
	bit_or(ret, &ka_under_attack_table[hash][pos]);
}
