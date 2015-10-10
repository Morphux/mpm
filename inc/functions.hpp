/*
 * functions.hpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-10 05:03
 */

#ifndef __FUNCTIONS__
# define __FUNCTIONS__

# include <list>
# include <string>
# include <sstream>
# include <stdio.h>
# include <ftw.h>
# include <unistd.h>

std::list<std::string>		split(std::string s, char delim);
int							rmrf(const char *path);

#endif
