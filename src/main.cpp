/*
 * main.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 04:38
 */

#include "main.hpp"

void	getOptions(int ac, char **av) {
	int				i, v, j;
	std::string		tmp, opt;

	Error::programName = std::string(av[0]);
	Options::getConfig();
	for (i = 1, v = 0; i < ac; i++, v = 0) {
		if (av[i][v++] == '-') {
			tmp = std::string(av[i]);
			if (tmp.length() > 1 && tmp[v] == '-')
				v++;
			for (j = v; tmp[j] != '=' && tmp[j]; j++);
			opt = tmp.substr(v, (j - v));
			if (tmp[j] == '=') {
				for (v = j; tmp[j]; j++);
				Options::addOptions(opt, tmp.substr(v + 1, (j - v)));
			} else {
				Options::addOptions(opt);
			}
		} else {
			Options::args.push_back(std::string(av[i]));
		}
	}
}

void	execute(void) {
	std::string cmd = *(Options::args.begin());

	Options::args.remove(*(Options::args.begin()));
	if (cmd == "install") {
		if (getuid())
			Error::error("Installation needs to be launch as root !");
		Download *d = new Download(Options::args);
		d->getAllPackages();
		d->installAll();
	} else if (cmd == "remove") {
		Manager		*m = new Manager(Options::args);
		m->uninstall();
	} else {
		Error::error("Unknown action: " + cmd);
	}
	Error::info("Quitting");
}

int		main(int ac, char **av) {
	getOptions(ac, av);
	if (Options::args.size())
		execute();
	else
		Error::error("No arguments provided.");
	return 0;
}
