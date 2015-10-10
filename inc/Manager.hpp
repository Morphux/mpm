/*
 * Manager.hpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-10 09:50
 */

#ifndef __MANAGER__
# define __MANAGER__

# include <string>
# include <list>
# include "Package.hpp"
# include "Exec.hpp"

class	Manager {
	public:
		Manager(std::list<std::string> args);
		~Manager(void);

		void	uninstall(void);

	private:
		std::list<Package *>	_package;
};

#endif
