void curse_mode_reset()
{
	store_select_y = store_select_x = -1;
	store_moti_y = store_moti_x = -1;
	store_pos = store_bbindex = -1;
	list_current_line = 0;
	list_start_line = 0;
	mode = SELECT;

	bit_clear(&selected);
	bit_clear(&reverse);
}

void curse_mode_restore(int m)
{
	int p, q;

	if (m == SELECT) { /* to MOTI */
		if (store_moti_x == -1) {
			p = MOTI_X;
			q = MOTI_Y;
		}
		else {
			p = store_moti_x;
			q = store_moti_y;
		}
		move(q, p);
		mode = MOTI;
	}
	else if (m == MOTI) { /* to SELECT */
		move(store_select_y, store_select_x);
		mode = SELECT;
	}
	else { /* from MOVE */
		if (store_mode == SELECT)
			move(store_select_y, store_select_x);
		else if (store_mode == MOTI)
			move(store_moti_y, store_moti_x);
		mode = store_mode;
	}
	list_current_line = 0;
	list_start_line = 0;
}

void curse_mode_save(int m)
{
	if (m == SELECT) {
		store_select_x = x;
		store_select_y = y;
		store_pos = curse_yx2pos(y, x);
	}
	else if (m == MOTI) {
		store_moti_x = x;
		store_moti_y = y;
		store_pos = -1;
	}

	store_mode = m;
}

/*
void curse_make_move()
{
	make_move(store_kh, &board);

	turn++;
	player = turn % 2;

	if (khead.kh != NULL) {
		free(khead.kh);
		init_array(&khead);
	}
	search(&board, &khead, player);

	//curse_printlist();
	curse_mode_reset();
	curse_move_firstpos(player);
}
*/

void curse_select_mode(int c)
{
	int p = x, q = y;

	switch (c) {
	case 'h':
	case KEY_LEFT:
		p -= INC_X;
		break;
	case 'j':
	case KEY_DOWN:
		q += INC_Y;
		break;
	case 'k':
	case KEY_UP:
		q -= INC_Y;
		break;
	case 'l':
	case KEY_RIGHT:
		p += INC_X;
		break;
	case 'm':
	case KEY_RET:
		if (list_lines > 0) {
			mode = MOVE;
			curse_mode_save(SELECT);
		}
		return;
	case 't':
		mode = MOTI;
		curse_mode_save(SELECT);
		curse_mode_restore(SELECT);
		return;
	case 'q':
		curse_die();
		return;
	}

	p = (p < LTOP_X) ? RTOP_X : (p > RTOP_X) ? LTOP_X : p;
	q = (q < LTOP_Y) ? LBOTTOM_Y : (q > LBOTTOM_Y) ? LTOP_Y : q;

	move(q, p);
}

void curse_moti_mode(int c)
{
	int p = x, q = y;

	switch (c) {
	case 'j':
	case KEY_DOWN:
		q += INC_SUB;
		break;
	case 'k':
	case KEY_UP:
		q -= INC_SUB;
		break;
	case 'm':
	case KEY_RET:
		if (list_lines > 0) {
			mode = MOVE;
			curse_mode_save(MOTI);
		}
		return;
	case 't':
		mode = SELECT;
		curse_mode_save(MOTI);
		curse_mode_restore(MOTI);
		return;
	case 'q':
		curse_die();
		return;
	}

	q = (q < SUB2_TOP_Y) ? SUB2_BOTTOM_Y : (q > SUB2_BOTTOM_Y) ? SUB2_TOP_Y : q;
	/*
	if (player == SENTE)
	else
		q = (q < SUB1_TOP_Y) ? SUB1_BOTTOM_Y : (q > SUB1_BOTTOM_Y) ? SUB1_TOP_Y : q;
	*/

	move(q, p);
}

int curse_move_mode(int c)
{
	int p, q, pos = curse_yx2pos(y, x);

	switch (c) {
	case 'j':
	case KEY_DOWN:
		list_current_line++;
		if (list_current_line >= list_start_line + LIST_LINES)
			list_start_line++;
		break;
	case 'k':
	case KEY_UP:
		list_current_line--;
		if (list_current_line < list_start_line)
			list_start_line--;
		break;
	case 'r':
	case KEY_RET:
		return TRUE;
		break;
	case 'e':
	case KEY_ESC:
		curse_mode_restore(MOVE);
		break;
	case 'q':
		curse_die();
		break;
	}

	if (list_current_line >= list_lines) {
		list_start_line = 0;
		list_current_line = 0;
	}
	else if (list_current_line < 0) {
		list_start_line = list_lines - LIST_LINES;
		list_current_line = list_lines - 1;
	}

	return FALSE;
}

void curse_insert_mode()
{
	/* not implemented */
}

kouho *curse_move(int c)
{
	switch (mode) {
	case SELECT:
		curse_select_mode(c);
		break;
	case MOTI:
		curse_moti_mode(c);
		break;
	case MOVE:
		if (curse_move_mode(c))
			return store_kh;
		break;
	case INSERT:
		curse_insert_mode();
		break;
	}

	return NULL;
}
