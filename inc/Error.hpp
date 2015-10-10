/*
 * Error.hpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 06:09
 */

#ifndef __ERROR__
# define __ERROR__

# include <iostream>
# include <unistd.h>
# include <list>
# include <fcntl.h>
# include "Exec.hpp"
# define YELLOW "\e[0;33m"
# define RED	"\e[0;31m"
# define GREEN	"\e[0;32m"
# define BLUE	"\e[0;34m"
# define PURP	"\e[0;35m"
# define DEF	"\e[0m"

class	Error {
	public:
		static void		info(std::string s);
		static void		warning(std::string s);
		static void		error(std::string s);
		static void		success(std::string s);
		static int		input(std::string s);
		static void		execError(void);
		static void		log(std::string s);

		static std::string		programName;
		static int				debugFlag;
};

#endif
