/* pos */
bitboard_t pos_to_bit[N * N];
unsigned int pos_to_index[N * N][2];
//unsigned int index_to_pos[N][N];

/* random table for hash key */
//hkey_t random_table[B][N * N + 19];

/* rank / file table */
bitboard_t rank_table[N];
bitboard_t file_table[N];

/* line table */
bitboard_t line_table[8][N * N];

/* jump table */
bitboard_t jump_table[B][N * N][N * N];
/*
bitboard_t ky_over_jump_table[N * N][N * N];
bitboard_t ky_under_jump_table[N * N][N * N];
bitboard_t ka_jump_table[N * N][N * N];
bitboard_t hi_jump_table[N * N][N * N];
*/

/* kiki table*/
bitboard_t kiki_table[B][N * N];

/* mask for kindergarten bitboards */
bitboard_t file_mask_table[N];
bitboard_t rank_mask_table[N];
bitboard_t over_mask_table[N * N];
bitboard_t under_mask_table[N * N];

/* kindergarten biboards */
bitboard_t hi_rank_attack_table[128][N * N];
bitboard_t ka_over_attack_table[128][N * N];
bitboard_t ka_under_attack_table[128][N * N];
bitboard_t hi_file_attack_table[256][N * N];
bitboard_t ky_over_attack_table[256][N * N];
bitboard_t ky_under_attack_table[256][N * N];
