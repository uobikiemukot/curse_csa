#include "../def.h"

kouho nobrain(state *board, int player, void *arg)
{
	int k;
	ktab khead;
	kouho kh;

	init_kouho(&kh);
	init_array(&khead);
	search(board, &khead, player);

	if (khead.num == 0)
		return	kh;

	kh = khead.kh[gen_rand32() % khead.num];
	free(khead.kh);

	return kh;
}

void main(int argc, char *argv[])
{
	/* var */
	int port = 4081;
	char *addr = "131.113.46.58";

	/* init */
	if (argc < 3) {
		printf("usage: ./csa USER PASSWD\n");
		exit(EXIT_FAILURE);
	}

	init_data();
	init_rand();

	/* csa-nobrain */
	csa(argv[1], argv[2], addr, port, nobrain, NULL);
}
