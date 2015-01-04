#include "../inc/mpm.h"

static void		print_line(int col) {
	int		i;

	for (i = 0; i < col; i++) {
		printf("-");
	}
	printf("\n");
}

static void		menu(int col) {
	int		i;

	printf(" \033[1;38mPACKAGE");
	col -= 8;
	for (i = col; i > (col / 2 + 8); i--)
		printf(" ");
	printf("VERSION");
	i -= 7;
	for (; i > 6; i--)
		printf(" ");
	printf("SIZE \033[0m\n");
}

static void		print_package_info(mpm_pinfo *p, int col) {
	int		i, tmp = col;
	while (p) {
		col = tmp;
		printf(" %s", p->name);
		col -= strlen(p->name);
		for (i = col; i > (col / 2 + 7); i--)
			printf(" ");
		printf("%s", p->version);
		i -= strlen(p->version);
		for (; i > strlen(p->size) + 2; i--)
			printf(" ");
		printf("%s \n", p->size);
		p = p->next;
	}
}

int		resume(mpm_pinfo *p) {
	struct winsize	w;
	char			buffer[1];

	printf("\033[0;33mRESUME:\033[0m\n");
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	print_line(w.ws_col);
	menu(w.ws_col);
	print_line(w.ws_col);
	print_package_info(p, w.ws_col);
	print_line(w.ws_col);
	write(1, "\033[1;32m-->\033[0m Sound's good to you ? [y/n] ", 43);
	read(0, buffer, 2);
	if (buffer[0] == 'y' || buffer[0] == '\n')
		return 0;
	else
		error("Aborting");
}
