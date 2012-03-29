enum {
	//KINIT = 600,
	KINIT = 38,
	KGROW = 2,
};

void init_array(ktab *khead)
{
	khead->num = khead->max = khead->count = 0;
	khead->kh = NULL;
}

void init_kouho(kouho *kh)
{
	kh->from = kh->from_bbindex = kh->to = kh->to_bbindex = kh->captured_bbindex = -1;
	kh->count = kh->sum = kh->sqr = kh->rate = 0;
	kh->value = 0.0;
	kh->child = NULL;
}

void add(kouho *new, ktab *khead)
{
	kouho *kh;

	if (khead->kh == NULL) {
		if ((khead->kh = (kouho *) malloc(KINIT * sizeof(kouho))) == NULL) {
			perror("malloc:");
			exit(EXIT_FAILURE);
		}
		khead->max = KINIT;
		khead->num = 0;
	}
	else if (khead->num >= khead->max) {
		if ((kh = (kouho *)
			realloc(khead->kh, khead->max * KGROW * sizeof(kouho))) == NULL) {
			printf("max:%d num:%d\n", khead->max, khead->num);
			perror("realloc:");
			exit(EXIT_FAILURE);
		}
		khead->max *= KGROW;
		khead->kh = kh;
	}

	khead->kh[khead->num] = *new;
	khead->num++;
}

void swap(kouho *kh, int i, int j)
{
	kouho tmp;
	tmp = kh[i];
	kh[i] = kh[j];
	kh[j] = tmp;
}

void sort(kouho *kh, int num)
{
	int i, last;

	if (num <= 1)
		return;

	swap(kh, 0, gen_rand32() % num);
	last = 0;

	for (i = 1; i < num; i++)
		if (kh[i].value > kh[0].value)
			swap(kh, ++last, i);

	swap(kh, 0, last);
	sort(kh, last);
	sort(kh + last + 1, num - last - 1);
}

kouho *max_sum(kouho *kh, int num)
{
	int i, index = 0;
	float max = -1.0;

	for (i = 0; i < num; i++) {
		if (max < kh[i].sum) {
			max = kh[i].sum;
			index = i;
		}
	}

	return &kh[index];
}

kouho *max_value(kouho *kh, int num)
{
	int i, index = 0;
	float max = -1.0;

	for (i = 0; i < num; i++) {
		if (max < kh[i].value) {
			max = kh[i].value;
			index = i;
		}
	}

	return &kh[index];
}

kouho *max_count(kouho *kh, int num)
{
	int i, index = 0, max = -1;

	for (i = 0; i < num; i++) {
		if (max < kh[i].count) {
			max = kh[i].count;
			index = i;
		}
	}

	return &kh[index];
}
