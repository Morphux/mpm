/*
 * Options.hpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 05:59
 */

#ifndef __OPTIONS__
# define __OPTIONS__

# include <map>
# include <list>
# include <iostream>
# include <json/json.h>
# include <fstream>
# include <sstream>
# include "Error.hpp"

class	Options {
	public:
		static void			addOptions(std::string opt);
		static void			addOptions(std::string opt, std::string val);
		static bool			isOptions(std::string s);
		static std::string	getOptions(std::string s);
		static void			getConfig(void);

		static std::map<std::string, std::string>	map;
		static std::list<std::string>				args;
		static Json::Value							config;
		static char * const							env[];
};

#endif
