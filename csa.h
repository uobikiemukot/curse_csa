enum {
	CSA_DEBUG = 0,
	CSA_EXMODE = 0,
	CSA_INFINITE = 0,
	CSA_BUF = 8192,
	CSA_LBUF = 256,
};

int fd; /* file descripter of socket */

void csa_sendmsg(char *buf)
{
	if (send(fd, buf, strlen(buf), 0) < 0) {
		perror("send");
		exit(EXIT_FAILURE);
	}
	if (CSA_DEBUG)
		printf("<%s", buf);
}

void init_csa(char *name, char *pass, char *addr, int port)
{
	struct sockaddr_in caddr;
	char lbuf[CSA_LBUF];

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	memset(&caddr, 0, sizeof(caddr));
	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr(addr);
	caddr.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *) &caddr, sizeof(caddr)) < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

	memset(lbuf, '\0', CSA_LBUF);

	if (CSA_EXMODE)
		snprintf(lbuf, CSA_LBUF, "LOGIN %s %s x1\n", name, pass); // extended mode
	else
		snprintf(lbuf, CSA_LBUF, "LOGIN %s %s\n", name, pass); // csa mode

	csa_sendmsg(lbuf);

	if (CSA_EXMODE)
		csa_sendmsg("%%GAME test-0-0 *\n");
}

void destroy_csa()
{
	close(fd);
}

char *type2csa(kouho *kh, char *tbuf)
{
	int type = bbindex2type(kh->to_bbindex);

	switch(type) {
	case OU:
		strncpy(tbuf, "OU", 3);
		break;
	case KI:
		strncpy(tbuf, "KI", 3);
		break;
	case FU:
		strncpy(tbuf, "FU", 3);
		break;
	case GI:
		strncpy(tbuf, "GI", 3);
		break;
	case KE:
		strncpy(tbuf, "KE", 3);
		break;
	case KY:
		strncpy(tbuf, "KY", 3);
		break;
	case HI:
		strncpy(tbuf, "HI", 3);
		break;
	case KA:
		strncpy(tbuf, "KA", 3);
		break;
	case TO:
		strncpy(tbuf, "TO", 3);
		break;
	case NG:
		strncpy(tbuf, "NG", 3);
		break;
	case NK:
		strncpy(tbuf, "NK", 3);
		break;
	case NY:
		strncpy(tbuf, "NY", 3);
		break;
	case RY:
		strncpy(tbuf, "RY", 3);
		break;
	case UM:
		strncpy(tbuf, "UM", 3);
		break;
	}

	return tbuf;
}

void csa_step(kouho (*func)(state *board, int player, void *arg),
	state *board, int player, void *arg)
{
	int i, j, k, l;
	char lbuf[CSA_LBUF], tbuf[3];
	kouho kh;

	kh = (*func)(board, player, arg);

	if (kh.to == -1) {
		csa_sendmsg("%TORYO\n");
	}
	else {
		if (CSA_DEBUG)
			printkouho(&kh);

		if (kh.from == -1)
			i = j = -1;
		else
			pos2index(kh.from, &i, &j);

		pos2index(kh.to, &k, &l);

		snprintf(lbuf, CSA_LBUF, "%c%d%d%d%d%s\n",
			(player == SENTE) ? '+' : '-',
			i + 1, j + 1, k + 1, l + 1,
			type2csa(&kh, tbuf));
		csa_sendmsg(lbuf);
	}
}

int csa2type(char *tbuf)
{
	return (strncmp(tbuf, "OU", 2) == 0) ? OU:
		(strncmp(tbuf, "KI", 2) == 0) ? KI:
		(strncmp(tbuf, "FU", 2) == 0) ? FU:
		(strncmp(tbuf, "GI", 2) == 0) ? GI:
		(strncmp(tbuf, "KE", 2) == 0) ? KE:
		(strncmp(tbuf, "KY", 2) == 0) ? KY:
		(strncmp(tbuf, "HI", 2) == 0) ? HI:
		(strncmp(tbuf, "KA", 2) == 0) ? KA:
		(strncmp(tbuf, "TO", 2) == 0) ? TO:
		(strncmp(tbuf, "NG", 2) == 0) ? NG:
		(strncmp(tbuf, "NK", 2) == 0) ? NK:
		(strncmp(tbuf, "NY", 2) == 0) ? NY:
		(strncmp(tbuf, "RY", 2) == 0) ? RY:
		UM;
}

int char2pos(int c)
{
	return (c - '0') - 1;
}

kouho csa2kouho(char *lbuf, state *board, int player) /* pseudo move */
{
	int i, type;
	char tbuf[3];
	bitboard_t tmp;
	kouho kh;

	init_kouho(&kh);
	memset(tbuf, '\0', sizeof(tbuf));

	strncpy(tbuf, &lbuf[5], 2);
	type = csa2type(tbuf);

	kh.to = char2pos(lbuf[3]) + char2pos(lbuf[4]) * N;
	kh.to_bbindex = type2bbindex(type, player);

	if (lbuf[1] != '0') {
		kh.from = char2pos(lbuf[1]) + char2pos(lbuf[2]) * N;
		for (i = player; i < B; i += 2) {
			pos_and3(&tmp, kh.from, &board->ban[i]);
			if (bit_test(&tmp)) {
				kh.from_bbindex = i;
				break;
			}
		}

		for (i = 1 - player; i < B; i += 2) {
			pos_and3(&tmp, kh.to, &board->ban[i]);
			if (bit_test(&tmp)) {
				kh.captured_bbindex = i;
				break;
			}
		}
	}

	return kh;
}

void csa_move(char *lbuf, state *board, int player)
{
	int i, type;
	char tbuf[3];
	bitboard_t tmp;
	kouho kh;

	init_kouho(&kh);
	memset(tbuf, '\0', sizeof(tbuf));

	strncpy(tbuf, &lbuf[5], 2);
	type = csa2type(tbuf);

	kh.to = char2pos(lbuf[3]) + char2pos(lbuf[4]) * N;
	kh.to_bbindex = type2bbindex(type, player);

	if (lbuf[1] == '0') {
		if (CSA_DEBUG)
			printf("type:%d player:%d tbuf:%s\n", type, player, tbuf);
	}
	else {
		kh.from = char2pos(lbuf[1]) + char2pos(lbuf[2]) * N;
		for (i = player; i < B; i += 2) {
			pos_and3(&tmp, kh.from, &board->ban[i]);
			if (bit_test(&tmp)) {
				kh.from_bbindex = i;
				break;
			}
		}

		for (i = 1 - player; i < B; i += 2) {
			pos_and3(&tmp, kh.to, &board->ban[i]);
			if (bit_test(&tmp)) {
				kh.captured_bbindex = i;
				break;
			}
		}
	}

	if (CSA_DEBUG)
		printkouho(&kh);

	make_move(&kh, board);

	if (CSA_DEBUG)
		printban(board);
}

void csa_readcmd(char *lbuf,
	kouho (*func)(state *board, int player, void *arg), void *arg)
{
	int i;
	static char id[CSA_LBUF], buf[CSA_LBUF];
	static int player = SENTE, turn = -1;
	static state board;

	if (strncmp(lbuf, "Game_ID:", 8) == 0) {
		if (CSA_EXMODE) {
			memset(id, '\0', CSA_LBUF);
			strncpy(id, &lbuf[8], CSA_LBUF);
			if (CSA_DEBUG)
				printf("##id:%s\n", id);
		}
	}
	else if (strncmp(lbuf, "Your_Turn:", 10) == 0) {
		if (lbuf[10] == '+')
			player = SENTE;
		else
			player = GOTE;

		if (CSA_DEBUG)
			printf("#player:%d\n", player);
	}
	else if (strncmp(lbuf, "END Game_Summary", 16) == 0) {
		csa_sendmsg("AGREE\n");
	}
	else if (strncmp(lbuf, "START:", 6) == 0) {
		turn = 0;
		init_ban(&board);

		if (player == SENTE)
			csa_step(func, &board, player, arg);
	}
	else if (turn >= 0 
		&& (strncmp(lbuf, "+", 1) == 0 || strncmp(lbuf, "-", 1) == 0)) {
		csa_move(lbuf, &board, turn % 2);
		turn++;

		if (player == turn % 2)
			csa_step(func, &board, player, arg);
	}
	else if (strncmp(lbuf, "#ILLEGAL", 8) == 0) {
		if (CSA_DEBUG && CSA_EXMODE) {
			snprintf(buf, CSA_LBUF, "%%%%SHOW %s\n", id);
			csa_sendmsg(buf);
		}
	}
	else if (strncmp(lbuf, "#WIN", 4) == 0
		|| strncmp(lbuf, "#LOSE", 5) == 0
		|| strncmp(lbuf, "#DRAW", 5) == 0) {
		if (CSA_INFINITE && CSA_EXMODE)
			csa_sendmsg("%%GAME test-0-0 *\n");
		else
			csa_sendmsg("LOGOUT\n");
	}
}

void csa_parse(char *buf,
	kouho (*func)(state *board, int player, void *arg), void *arg)
{
	char lbuf[CSA_LBUF];
	int i = 0, len = strlen(buf);
	char *cp, *lp, *endp;

	if (CSA_DEBUG)
		printf(">%s", buf);
	endp = &buf[len - 1];
	cp = buf;

start:
	memset(lbuf, '\0', CSA_LBUF);
	lp = lbuf;

	while (*cp != '\n' && cp != endp)
		*lp++ = *cp++;

	*lp = '\0';
	csa_readcmd(lbuf, func, arg);

	if (*cp == '\0' || cp == endp)
		return;

	cp++;

	goto start;
}

int csa_getmsg(char *buf)
{
	int res;

	memset(buf, '\0', CSA_BUF);
	res = recv(fd, buf, CSA_BUF, 0);

	if (res < 0) {
		if (CSA_DEBUG)
			perror("recv");
		//exit(EXIT_FAILURE);
		return FALSE;
	}
	else if (res == 0)
		//exit(EXIT_SUCCESS);
		return FALSE;

	return TRUE;
}

void csa(char *name, char *passwd, char *addr, int port,
	kouho (*func)(state *board, int player, void *arg), void *arg)
{
	char buf[CSA_BUF];

	init_csa(name, passwd, addr, port);

	for (;;) {
		csa_getmsg(buf);
		csa_parse(buf, func, arg);
	}

	close(fd);
}
