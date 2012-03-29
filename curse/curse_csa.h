int fd; /* file descripter of socket */

void curse_sendmsg(char *buf)
{
	if (send(fd, buf, strlen(buf), 0) < 0)
		wprintw(srv, "!error: send()\n", buf);

	if (CURSE_DEBUG)
		wprintw(srv, "<%s", buf);
}

void curse_init_csa(char *name, char *pass, char *addr, int port)
{
	struct sockaddr_in caddr;
	char lbuf[CSA_LBUF];

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		wprintw(srv, "!error: socket()\n");

	memset(&caddr, 0, sizeof(caddr));
	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr(addr);
	caddr.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *) &caddr, sizeof(caddr)) < 0)
		wprintw(srv, "!error: connect()\n");

	memset(lbuf, '\0', CSA_LBUF);

	if (CSA_EXMODE)
		snprintf(lbuf, CSA_LBUF, "LOGIN %s %s x1\n", name, pass); // extended mode
	else
		snprintf(lbuf, CSA_LBUF, "LOGIN %s %s\n", name, pass); // csa mode

	curse_sendmsg(lbuf);

	if (CSA_EXMODE)
		curse_sendmsg("%%GAME test-0-0 *\n");
}

void curse_destroy_csa()
{
	close(fd);
}

void curse_csa_step(kouho (*func)(state *board, int player, void *arg),
	state *board, int player, void *arg)
{
	int i, j, k, l;
	char lbuf[CSA_LBUF], tbuf[3];
	kouho kh;

	kh = (*func)(board, player, arg);

	if (kh.to == -1) {
		curse_sendmsg("%TORYO\n");
	}
	else {
		if (kh.from == -1)
			i = j = -1;
		else
			pos2index(kh.from, &i, &j);

		pos2index(kh.to, &k, &l);

		snprintf(lbuf, CSA_LBUF, "%c%d%d%d%d%s\n",
			(player == SENTE) ? '+' : '-',
			i + 1, j + 1, k + 1, l + 1,
			type2csa(&kh, tbuf));
		curse_sendmsg(lbuf);
	}
}

void curse_readcmd(char *lbuf,
	kouho (*func)(state *board, int player, void *arg), void *arg)
{
	int i;
	static char id[CSA_LBUF], buf[CSA_LBUF], sente[CSA_LBUF], gote[CSA_LBUF];
	static int player = SENTE, turn = -1;
	static state board;
	kouho kh;

	if (strncmp(lbuf, "Game_ID:", 8) == 0) {
		if (CSA_EXMODE) {
			memset(id, '\0', CSA_LBUF);
			strncpy(id, &lbuf[8], CSA_LBUF);
		}
	}
	else if (strncmp(lbuf, "Name", 4) == 0) {
		if (lbuf[4] == '+') {
			memset(sente, '\0', CSA_LBUF);
			strncpy(sente, &lbuf[6], CSA_LBUF);
		}
		else {
			memset(gote, '\0', CSA_LBUF);
			strncpy(gote, &lbuf[6], CSA_LBUF);
		}
	}
	else if (strncmp(lbuf, "Your_Turn:", 10) == 0) {
		if (lbuf[10] == '+') {
			player = SENTE;
		}
		else {
			player = GOTE;
		}
		mvprintw(SENTE_Y, SENTE_X, "先手:%s%c", sente, (player == SENTE) ? '*': ' ');
		mvprintw(SENTE_Y + 1, SENTE_X, "後手:%s%c", gote, (player == GOTE) ? '*': ' ');
	}
	else if (strncmp(lbuf, "END Game_Summary", 16) == 0) {
		curse_sendmsg("AGREE\n");
	}
	else if (strncmp(lbuf, "START:", 6) == 0) {
		turn = 0;
		init_ban(&board);

		if (player == SENTE)
			curse_csa_step(func, &board, player, arg);
	}
	else if (turn >= 0 
		&& (strncmp(lbuf, "+", 1) == 0 || strncmp(lbuf, "-", 1) == 0)) {
		kh = csa2kouho(lbuf, &board, turn % 2);
		curse_printkouho(srv, &kh);
		wprintw(srv, "\n");

		csa_move(lbuf, &board, turn % 2);
		turn++;

		if (player == turn % 2)
			curse_csa_step(func, &board, player, arg);
	}
	else if (strncmp(lbuf, "#WIN", 4) == 0
		|| strncmp(lbuf, "#LOSE", 5) == 0
		|| strncmp(lbuf, "#DRAW", 5) == 0) {
		if (CSA_INFINITE && CSA_EXMODE)
			curse_sendmsg("%%GAME test-0-0 *\n");
		else
			curse_sendmsg("LOGOUT\n");
	}
}

void curse_parse(char *buf,
	kouho (*func)(state *board, int player, void *arg), void *arg)
{
	char lbuf[CSA_LBUF];
	int i = 0, len = strlen(buf);
	char *cp, *lp, *endp;

	if (CURSE_DEBUG)
		wprintw(srv, ">%s", buf);
	endp = &buf[len - 1];
	cp = buf;

start:
	memset(lbuf, '\0', CSA_LBUF);
	lp = lbuf;

	while (*cp != '\n' && cp != endp)
		*lp++ = *cp++;

	*lp = '\0';
	curse_readcmd(lbuf, func, arg);

	if (*cp == '\0' || cp == endp)
		return;

	cp++;

	goto start;
}

int curse_getmsg(char *buf)
{
	int res;

	memset(buf, '\0', CSA_BUF);
	res = recv(fd, buf, CSA_BUF, 0);

	if (res < 0)
		return FALSE;
	else if (res == 0)
		return FALSE;

	return TRUE;
}

void curse_csa(char *name, char *passwd, char *addr, int port,
	kouho (*func)(state *board, int player, void *arg), void *arg)
{
	char buf[CSA_BUF];

	curse_init_csa(name, passwd, addr, port);

	for (;;) {
		curse_getmsg(buf);
		curse_parse(buf, func, arg);

		wrefresh(srv);
		refresh();
	}

	curse_destroy_csa();
}
