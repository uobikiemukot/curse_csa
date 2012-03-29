void eprintf(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

FILE *eopen(char *path, char *mode)
{
	FILE *fp;

	if ((fp = fopen(path, mode)) == NULL)
		eprintf("fopen");

	return fp;
}

void *emalloc(size_t n)
{
	void *p;

	if ((p = malloc(n)) == NULL)
		eprintf("malloc");

	return p;
}
