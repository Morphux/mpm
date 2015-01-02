#include "../inc/mpm.h"

mpm_package		*add_package(mpm_package *package, char *name) {
	mpm_package		*tmp, *tmp2;

	tmp2 = package;
	tmp = malloc(sizeof(mpm_package));
	tmp->package = name;
	tmp->url = NULL;
	tmp->version = 0;
	tmp->next = NULL;
	if (package == NULL)
		return tmp;
	while (tmp->next)
		tmp2 = tmp2->next;
	tmp2->next = tmp;
	return package;
}

void	print_list(mpm_package *package) {
	while (package) {
		printf("%s\n", package->package);
		package = package->next;
	}
}
