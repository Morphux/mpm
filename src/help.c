#include "../inc/mpm.h"

void	help(void) {
	printf("mpm, The Morphux Package Manager.\n");
	printf("Version: %s\n", VERSION);
	printf("Actions:\n");
	printf("\tinstall: \tInstall a package\n");
	printf("\tremove: \tRemove a package\n");
	printf("\tget: \t\tGet the source of a package\n");
	printf("\tupdate: \tUpdate the packages\n");
	_exit(1);
}
