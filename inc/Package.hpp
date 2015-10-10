/*
 * Package.hpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 12:50
 */

#ifndef __PACKAGE__
# define __PACKAGE__

# include <json/json.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <archive.h>
# include <dirent.h>
# include <archive_entry.h>
# include <list>
# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include "Error.hpp"
# include "Options.hpp"
# include "Exec.hpp"
# include "functions.hpp"

class	Package {
	public:
		Package(std::string name, Json::Value init);
		Package(std::string name);
		~Package(void);

		void	install(void);
		void	decompress(void);
		void	readPackageInfos(void);
		void	patches(void);
		void	before(void);
		void	configure(void);
		void	make(void);
		void	minstall(void);
		void	after(void);
		void	save(void);
		void	clean(void);
		void	uninstall(void);

		void						setArchivePath(std::string s);
		void						setToDownload(int s);
		void						setExec(Exec *e);
		std::string					getVersion(void);
		std::string					getName(void);
		std::string					getUrl(void);
		std::list<std::string>		getNeededDeps(void);
		std::list<std::string>		getOptionnalDeps(void);
		float						getSize(void);
		int							getToDownload(void);
		int							getToInstall(void);

	private:
		std::string		_version;
		std::string		_packageUrl;
		std::string		_name;
		float			_size;
		std::string		_archive;
		std::string		_dir;
		std::list<std::string>	_neededDeps;
		std::list<std::string>	_optDeps;
		int						_toDownload;
		int						_toInstall;
		Json::Value		_infos;
		Exec			*_exec;

};

#endif
