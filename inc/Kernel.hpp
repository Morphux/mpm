/*
 * Kernel.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-13-12 23:38
 */

#ifndef __KERNEL__
# define __KERNEL__

# include <sys/utsname.h>
# include <iostream>
# include <fstream>
# include <time.h>
# include "Error.hpp"
# include "Exec.hpp"
# include "functions.hpp"

typedef struct		s_kern {
	std::string		name;
	std::string		value;
	int				isComment;
}					t_kern;

class	Kernel {
	public:
		Kernel(void);
		~Kernel(void);

		void		setConfig(std::string name, std::string value);
		void		make(void);
		void		install(void);

		static int	recompile;

	private:
		void		_readConfig(std::string s);

		std::list<t_kern *>	_config;
};

#endif
