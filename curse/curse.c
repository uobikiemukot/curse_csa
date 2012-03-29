#include "curse_common.h"
#include "curse_print.h"
#include "curse_util.h"
#include "curse_move.h"
#include "curse_info.h"
#include "curse_csa.h"

kouho curse_step(state *board, int player, void *arg)
{
	kouho *kh;

	wrefresh(srv);
	refresh();

	curse_get_state(board, player);
	curse_mode_reset();

	while (1) {
		curse_setinfo();
		curse_printban();
		if ((kh = curse_move(getch())) != NULL)
			break;

		refresh();
	}

	return *kh;
}

int main(int argc, char *argv[])
{
	int port = 4081;
	char *addr = "131.113.46.58";

	/* check arg */
	if (argc < 3) {
		printf("usage: ./curse USER PASS\n");
		exit(EXIT_FAILURE);
	}

	/* init */
	curse_init();

	/* main */
	curse_csa(argv[1], argv[2], addr, port, curse_step, NULL);
	curse_die();
}
