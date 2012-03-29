int bbindex2type(int bbindex)
{
	return (int)(bbindex >> 1);
}

int bbindex2player(int bbindex)
{
	return (int)(bbindex % 2);
}

int bbindex2reverse(int bbindex)
{
	int type = bbindex2type(bbindex), player = 1 - bbindex2player(bbindex);

	return (type << 1) + player;
}

int bbindex2moti(int bbindex)
{
	int type, player = 1 - bbindex2player(bbindex);

	if (bbindex >= S_TO)
		bbindex -= PROMOTE;

	type = bbindex2type(bbindex);

	return (type << 1) + player;
}

int type2bbindex(int type, int player)
{
	return (type << 1) + player;
}

int calc_rate(int bbindex)
{
	int type = bbindex2type(bbindex);

	return (type == OU) ? 8192:
		(type == KI) ? 6:
		(type == FU) ? 1:
		(type == GI) ? 5:
		(type == KE) ? 4:
		(type == KY) ? 3:
		(type == HI) ? 10:
		(type == KA) ? 8:
		(type == TO) ? 7:
		(type == NG) ? 6:
		(type == NK) ? 6:
		(type == NY) ? 6:
		(type == RY) ? 12:
		10; // UM
}

