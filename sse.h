#include <emmintrin.h> /* SSE2 */
//#include <smmintrin.h> /* SSE4 */

/*
	need SSE2 at least
*/

enum { /* sse flag */
	SSE4 = 0,
};

//typedef __m128i bitboard_t;
typedef union bitboard bitboard_t;
union bitboard {
	unsigned int p[4];
	__m128i m;
};
