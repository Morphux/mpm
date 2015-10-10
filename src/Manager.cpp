/*
 * Manager.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-10 09:50
 */

#include "Manager.hpp"

Manager::Manager(std::list<std::string> args) {
	std::list<std::string>::iterator	it;
	Package								*p;
	Exec								*exec = new Exec();

	for (it = args.begin(); it != args.end(); it++) {
		p = new Package(*it);
		p->setExec(exec);
		this->_package.push_back(p);
	}
}

Manager::~Manager(void) {
	return ;
}

void	Manager::uninstall(void) {
	std::list<Package *>::iterator	it;

	for (it = this->_package.begin(); it != this->_package.end(); it++) {
		if (!(*it)->getToInstall()) {
			Error::warning("The package " + (*it)->getName() + " is not installed");
		} else {
			(*it)->uninstall();
		}
	}
}
