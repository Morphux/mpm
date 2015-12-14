/*
 * Developer.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-13-12 19:17
 */

#ifndef __DEVELOPER__
# define __DEVELOPER__

# include <sys/types.h>
# include <sys/stat.h>
# include <archive.h>
# include <dirent.h>
# include <archive_entry.h>
# include <curl/curl.h>
# include <iostream>
# include <fstream>
# include <stdio.h>
# include <list>
# include "Error.hpp"

# define BASIC_FILE "{\n\
	\"package\": {\n\
		\"name\": \"\",\n\
		\"version\": \"\",\n\
		\"description\": \"\",\n\
	},\n\
	\"compilation\": {\n\
		\"before\": [],\n\
		\"patches\": [],\n\
		\"configure\": [],\n\
		\"make\": \"\",\n\
		\"install\": \"install\",\n\
		\"uninstall\": \"uninstall\",\n\
	},\n\
	\"dependencies\": {\n\
		\"needed\": [],\n\
		\"recommended\": [],\n\
		\"optionnal\": []\n\
	}\n\
}"

class	Developer {
	public:
		Developer(void);
		~Developer(void);

		void		createPackage(std::list<std::string>);
		std::string		getArchive(std::string url);
		std::string	extract(std::string archive);

};

#endif
