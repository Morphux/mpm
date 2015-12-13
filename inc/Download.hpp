/*
 * Download.hpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 08:10
 */

#ifndef __DOWNLOAD__
# define __DOWNLOAD__

# define BASE_URL "http://api.morphux.org/"

# include <list>
# include <iostream>
# include <string>
# include <curl/curl.h>
# include <json/json.h>
# include <stdlib.h>
# include <stdio.h>
# include <iomanip>
# include "Error.hpp"
# include "Package.hpp"
# include "Exec.hpp"

class	Download {
	public:
		Download(std::list<std::string>);
		Download(std::string);
		~Download(void);

		void	getAllPackages(void);
		void	installAll(void);

		static float		totalSize;
		static float		currentSize;
		static float		currentTotalSize;
		static int			packCount;
		static int			currentCount;

	private:
		void			_getInfo(void);
		std::string		_getPackagesInfo(std::string url);
		static size_t	_curlCallback(void *contents, size_t size, size_t nmeb, void *userp);
		void			_createPackages(std::string json);
		void			_addPackage(std::string name);
		std::string		_getArchive(std::string url);
		static size_t	_myFwrite(void *buffer, size_t size, size_t nmemb, FILE *fd);
		static int		_progress(void* ptr, double TotalToDownload, double NowDownloaded, 
			double TotalToUpload, double NowUploaded);

		std::list<std::string>		_packages;
		std::string					_test;
		Json::Value					_response;
		std::list<Package *>		_packList;
		std::list<Package *>		_packTreated;
};

#endif
