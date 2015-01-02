#include "../inc/mpm.h"

static void	_init_opt(void) {
	options.install = 0;
	options.remove = 0;
	options.update = 0;
	options.get = 0;
	options.package = NULL;
}

void	get_opt(int ac, char **av) {
	int		i;

	_init_opt();
	for (i = 1; i < ac; i++) {
		if (i == 1) {
			if (!strcmp(av[i], "install"))
				options.install = 1;
			else if (!strcmp(av[i], "remove"))
				options.remove = 1;
			else if (!strcmp(av[i], "update"))
				options.update = 1;
			else if (!strcmp(av[i], "get"))
				options.get = 1;
			else
				help();
		} else {
			options.package = add_package(options.package, av[i]);
		}
	}
}
