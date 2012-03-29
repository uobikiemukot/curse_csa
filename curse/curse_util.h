int curse_yx2pos(int y, int x)
{
	int p, q;

	p = (x - LTOP_X) / INC_X;
	q = (y - LTOP_Y) / INC_Y;


	if (player == SENTE)
		// (LTOP_Y, LTOP_X) = pos:8
		return q * N + (N - p - 1);
	else
		// (LTOP_Y, LTOP_X) = pos:71
		return (N - q - 1) * N  + p;
}

void curse_pos2yx(int pos, int *y, int *x)
{
	if  (player == SENTE) {
		// (y, x)
		// pos:8 = (0, 0)
		// pos:7 = (0, 1)
		// pos:6 = (0, 2)
		// ...
		// pos:2 = (0, 6) 18
		// pos:1 = (0, 7) 21
		// pos:0 = (0, 8) 24
		*x = (N - (pos % N) - 1) * INC_X + LTOP_X;
		*y = (pos / N) * INC_Y + LTOP_Y;
	}
	else {
		// pos:72 = (0, 1)
		// pos:73 = (0, 2)
		// pos:74 = (0, 3)
		// ...
		// pos:78 = (0, 6)
		// pos:79 = (0, 7)
		// pos:80 = (9, 8)
		*x = (pos % N) * INC_X + LTOP_X;
		*y = (N - (pos / N) - 1) * INC_Y + LTOP_Y;
	}
}

void curse_search()
{
	if (khead.kh != NULL) {
		free(khead.kh);
		init_array(&khead);
	}
	search(board, &khead, player);
}

void curse_move_firstpos()
{
	int pos, p, q;
	//bitboard_t *bp = &board->uni[player];
	bitboard_t *bp = &board->ban[player];

	//pos = (player == SENTE) ? bit_first(bp): bit_last(bp);
	//pos = bit_first(bp);
	pos = bit_first(bp);
	curse_pos2yx(pos, &q, &p);
	move(q, p);
}

void curse_get_state(state *b, int p)
{
	board = b;
	player = p;

	curse_move_firstpos();
	curse_search();
}

void curse_die()
{
	signal(SIGINT, SIG_IGN);

	mvcur(0, COLS - 1, LINES - 1, 0);
	endwin();

	exit(EXIT_SUCCESS);
}

void curse_init()
{
	signal(SIGINT, curse_die);

	setlocale(LC_ALL, "");

	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLUE, COLOR_WHITE);
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	sub1 = subwin(stdscr, SUB_LINES, SUB_COLS, SUB1_Y, SUB1_Y);
	ban = subwin(stdscr, BAN_LINES, BAN_COLS, BAN_Y, BAN_X);
	sub2 = subwin(stdscr, SUB_LINES, SUB_COLS, SUB2_Y, SUB2_X);
	list = subwin(stdscr, LIST_LINES, LIST_COLS, LIST_Y, LIST_X);
	srv = subwin(stdscr, SRV_LINES, SRV_COLS, SRV_Y, SRV_X);
	if (CURSE_DEBUG)
		dump = subwin(stdscr, DUMP_LINES, DUMP_COLS, DUMP_Y, DUMP_X);

	/*
	wbkgd(sub1, COLOR_PAIR(1));
	wbkgd(ban, COLOR_PAIR(1));
	wbkgd(sub2, COLOR_PAIR(1));
	wbkgd(list, COLOR_PAIR(2));
	wbkgd(srv, COLOR_PAIR(2));
	wbkgd(dump, COLOR_PAIR(2));
	*/

	mvprintw(MODE_Y, MODE_X, "[mode] WAIT");
	mvprintw(LIST_Y - 1, LIST_X, "[possible movement(s)]");
	mvprintw(SRV_Y - 1, SRV_X, "[server message]");
	if (CURSE_DEBUG)
		mvprintw(DUMP_Y - 1, DUMP_X, "[debug]");

	scrollok(list, TRUE);
	scrollok(srv, TRUE);
	idlok(list, TRUE);

	bit_clear(&selected);
	bit_clear(&reverse);
	init_data();
	init_rand();
	init_array(&khead);
	//init(&board);
	//init_array(&khead);
	//search(&board, &khead, player);

	refresh();
}
