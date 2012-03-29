#include "../def.h"

void main(int argc, char *argv[])
{
	int port = 4081;
	char *addr = "131.113.46.58",
		buf[CSA_BUF], lbuf[CSA_LBUF];

	if (argc < 3)
		exit(EXIT_FAILURE);

	init_csa(argv[1], argv[2], addr, port);

	for (;;) {
		csa_getmsg(buf);
		printf(">%s", buf);

		memset(lbuf, '\0', CSA_LBUF);
		printf("<");
		if (fgets(lbuf, CSA_LBUF, stdin) != NULL) {
			if (send(fd, lbuf, strlen(lbuf), 0) < 0) {
				perror("send");
				exit(EXIT_FAILURE);
			}
		}
	}

	close(fd);
}
