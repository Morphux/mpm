#include "../inc/mpm.h"

static char	*return_string(char *str) {
	char	*dest = NULL;
	if (str) {
		dest = malloc(strlen(str) + 1);
		strcpy(dest, str);
		return dest;
	}
	return NULL;
}

mpm_pinfo	*parse(char *str, mpm_pinfo *packages) {
	char		**conf = ft_strsplit(str, '\n'), **tmp;
	int			i;
	mpm_pinfo	*pack_tmp;

	pack_tmp = malloc(sizeof(mpm_pinfo));
	for (i = 0; conf[i]; i++) {
		tmp = ft_strsplit(conf[i], '?');
		if (!strcmp(tmp[0], "name")) {
			pack_tmp->name = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "dependencies")) {
			pack_tmp->dependencies = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "version")) {
			pack_tmp->version = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "before")) {
			pack_tmp->before = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "url")) {
			pack_tmp->url = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "configure")) {
			pack_tmp->configure = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "make")) {
			pack_tmp->make = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "check")) {
			pack_tmp->check = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "install")) {
			pack_tmp->install = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "after")) {
			pack_tmp->after = return_string(tmp[1]);
		} else if (!strcmp(tmp[0], "authors")) {
			pack_tmp->after = return_string(tmp[1]);
		}
	}
	if (i == 0)
		error("Config file error");
	pack_tmp->next = packages;
	success("Configuration file: done. Looking for dependencies...");
	if (pack_tmp->dependencies) {
		conf = ft_strsplit(pack_tmp->dependencies, ',');
		for (i = 0; conf[i]; i++) {
			info(strjoin(strjoin("Look's like the package ", pack_tmp->name), " have dependencies."));
			pack_tmp = get_info(conf[i], pack_tmp);
		}
	}
	return pack_tmp;
}

void	print_packages(mpm_pinfo *p) {
	while (p) {
		printf("Name: %s\n", p->name);
		printf("URL: %s\n", p->url);
		printf("Version: %s\n", p->version);
		printf("Configure: %s\n", p->configure);
		printf("Make: %s\n", p->make);
		printf("Install: %s\n\n", p->install);
		p = p->next;
	}
}
