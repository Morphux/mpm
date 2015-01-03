/**
 * Morphux Package Manager
 * Release V.1
 * See INSTALL, AUTHORS and NEWS for more information
 * http://morphux.org
 */

#include "../inc/mpm.h"

static void	do_it(void) {
	if (options.install == 1) {
		install(options.package);
	}
}

int		main(int ac, char **av) {
	if (ac == 1)
		help();
	else
		get_opt(ac, av);
	do_it();
	return 0;
}
