/* bit function */
void bit_and(bitboard_t *first, const bitboard_t *second) 
{
	first->m = _mm_and_si128(first->m, second->m);	
}

void bit_and3(bitboard_t *ret, const bitboard_t *first, const bitboard_t *second) 
{
	ret->m = _mm_and_si128(first->m, second->m);	
}

void bit_or(bitboard_t *first, const bitboard_t *second) 
{
	first->m = _mm_or_si128(first->m, second->m);	
}

void bit_or3(bitboard_t *ret, const bitboard_t *first, const bitboard_t *second) 
{
	ret->m = _mm_or_si128(first->m, second->m);	
}

void bit_xor3(bitboard_t *ret, const bitboard_t *first, const bitboard_t *second) 
{
	ret->m = _mm_xor_si128(first->m, second->m);	
}

void bit_clear(bitboard_t *bit)
{
	bit->m = _mm_setzero_si128();
}

void bit_set(bitboard_t *bit, int u0, int u1, int u2)
{
	bit->m = _mm_set_epi32(0, u2, u1, u0);
}

void bit_not(bitboard_t *first, bitboard_t *second)
{
	/*
		LSB                                 MSB
		1111 1111 1111 1111 1111 1111 1110 0000
	*/
	first->m = _mm_andnot_si128(second->m, _mm_set_epi32(0x0, 0x7ffffff, 0x7ffffff, 0x7ffffff));
}

void bit_notand(bitboard_t *ret, bitboard_t *first, bitboard_t *second)
{
	// r = (~a) & b
	ret->m = _mm_andnot_si128(first->m, second->m);
}

void bit_copy(bitboard_t *first, bitboard_t *second)
{
	first->m = second->m;
}

int bit_zero(const bitboard_t *bit)
{
	if (SSE4)
		return _mm_testz_si128(bit->m, _mm_set1_epi32(0x7ffffff));
	else
		return (!bit->p[0] && !bit->p[1] && !bit->p[2]);
}

int bit_test(const bitboard_t *bit)
{
	if (SSE4)
		//return (!_mm_testz_si128(bit->m, _mm_set1_epi32(0x7ffffff)));
		return (!_mm_testc_si128(_mm_setzero_si128(), bit->m));
	else
		return (bit->p[0] | bit->p[1] | bit->p[2]);
}

int bit_equal(bitboard_t *first, bitboard_t *second)
{
	if (SSE4)
		return ( _mm_testc_si128(first->m, second->m) && _mm_testc_si128(second->m, first->m));
	else
		return (first->p[0] == second->p[0] && first->p[1] == second->p[1] && first->p[2] == second->p[2]);
}

int bit_first(const bitboard_t *bit)
{
	if (bit->p[0])
		return (__builtin_ctz(bit->p[0]));
	else if (bit->p[1])
		return (__builtin_ctz(bit->p[1]) + 27);
	else
		return (__builtin_ctz(bit->p[2]) + 54);
}

int bit_last(const bitboard_t *bit)
{
	if (bit->p[2])
		return (80 - (__builtin_clz(bit->p[2]) - 5));
	else if (bit->p[1])
		return (53 - (__builtin_clz(bit->p[1]) - 5));
	else
		return (26 - (__builtin_clz(bit->p[0]) - 5));
}


int popcount(int i)
{
	int ret = 0;

	while (i) {
		i &= i - 1;
		ret++;
	}

	return ret;
}

int bit_count(const bitboard_t *bit)
{
	//return (__builtin_popcount(bit->p[0]) + __builtin_popcount(bit->p[1]) + __builtin_popcount(bit->p[2]));
	return (popcount(bit->p[0]) + popcount(bit->p[1]) + popcount(bit->p[2]));
}

void bit_create(bitboard_t *occ, const bitboard_t bit[B])
{
	int i;

	bit_clear(occ);
	for (i = 0; i < B; i++)
		bit_or(occ, &bit[i]);
}

void bit_createall(state *board)
{
	int i;

	bit_clear(&board->uni[SENTE]);
	bit_clear(&board->uni[GOTE]);
	bit_clear(&board->uni[OCC]);

	for (i = 0; i < B; i++) {
		bit_or(&board->uni[OCC], &board->ban[i]);

		if ((i % 2) == 0)
			bit_or(&board->uni[SENTE], &board->ban[i]);
		else 
			bit_or(&board->uni[GOTE], &board->ban[i]);
	}
}

void bit_print(const bitboard_t *bit)
{
	int i, j;
	unsigned int shift;

	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			shift = i * N - j;
			if (shift < 27)
				printf("%d ", ((1 << shift) & bit->p[0]) ? 1: 0);
			else if (shift < 54)
				printf("%d ", ((1 << (shift - 27)) & bit->p[1]) ? 1: 0);
			else
				printf("%d ", ((1 << (shift - 54)) & bit->p[2]) ? 1: 0);
		}
		printf("\n");
	}
	printf("\n");
}

void bit_printall(const bitboard_t *bit)
{
	int i, j;
	unsigned int shift;

	printf("0                                                   26\n");
	for (i = 0; i < 128; i++) {
		if (i < 32)
			printf("%d ", ((1 << i) & bit->p[0]) ? 1: 0);
		else if (i < 64)
			printf("%d ", ((1 << (i - 32)) & bit->p[1]) ? 1: 0);
		else if (i < 96)
			printf("%d ", ((1 << (i - 64)) & bit->p[2]) ? 1: 0);
		else
			printf("%d ", ((1 << (i - 96)) & bit->p[3]) ? 1: 0);

		if ((i % 32) == 31) 
			printf("\n");
	}
	printf("\n");
}

/* index table */
void pos2index(int pos, int *i, int *j)
{
	*i = pos_to_index[pos][0];
	*j = pos_to_index[pos][1];
}

/* pos function */
void pos_xor(int pos, bitboard_t *bit)
{
	bit->m = _mm_xor_si128(pos_to_bit[pos].m, bit->m);	
	//c &= c - 1
	//_mm_and_si128(bit->m, _mm_sub_epi32(bit->m, foo!));
}

void pos_xor3(bitboard_t *first, int pos, bitboard_t *second)
{
	first->m = _mm_xor_si128(pos_to_bit[pos].m, second->m);	
}

void pos_or(int pos, bitboard_t *bit)
{
	bit->m = _mm_or_si128(pos_to_bit[pos].m, bit->m);	
}

void pos_or3(bitboard_t *first, int pos, bitboard_t *second)
{
	first->m = _mm_or_si128(pos_to_bit[pos].m, second->m);	
}

void pos_and3(bitboard_t *ret, int pos, const bitboard_t *bit)
{
	ret->m = _mm_and_si128(pos_to_bit[pos].m, bit->m);
}
