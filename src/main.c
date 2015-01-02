/**
 * Morphux Package Manager
 * Release V.1
 * See INSTALL, AUTHORS and NEWS for more information
 * http://morphux.org
 */

#include "../inc/mpm.h"

int		main(int ac, char **av) {
	if (ac == 1)
		help();
	else
		get_opt(ac, av);
	print_list(options.package);
	return 0;
}
