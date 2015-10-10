/*
 * Exec.hpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-09 02:26
 */

#ifndef __EXEC__
# define __EXEC__

# include <stdlib.h>
# include <string>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <list>
# include <sstream>
# include <iostream>
# include <unistd.h>
# include "Error.hpp"
# include "Options.hpp"
# include "functions.hpp"

class	Exec {
	public:
		Exec(void);
		~Exec(void);

		void	execute(std::list<std::string> args);

	private:
		const char						**_listToArray(std::list<std::string> args);

		std::list<std::string>		_path;
};

#endif
