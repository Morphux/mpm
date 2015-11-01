/*
 * Download.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 08:09
 */

#include "Download.hpp"

Download::Download(std::list<std::string> pack) : _packages(pack) {
	if (this->_packages.size() > 0)
		this->_getInfo();
	else
		Error::warning("Need arguments !");
	return ;
}

Download::Download(std::string path) {
	std::list<std::string>				tmp;
	std::list<std::string>::iterator	it;
	Package		*p = new Package(path, 1);

	if (p->getNeededDeps().size() > 0) {
		tmp = p->getNeededDeps();
		for (it = tmp.begin(); it != tmp.end(); it++)
			this->_packages.push_back(*it);
	}
	if (p->getRecDeps().size() > 0) {
		tmp = p->getRecDeps();
		for (it = tmp.begin(); it != tmp.end(); it++)
			this->_packages.push_back(*it);
	}
	if (this->_packages.size() > 0) {
		this->_getInfo();
		this->getAllPackages();
	}
	this->_packList.push_back(p);
}

Download::~Download(void) {
	return ;
}

void	Download::_getInfo(void) {
	std::string		url, tmp;
	std::list<std::string>::iterator	it;

	for (it = this->_packages.begin(); it != this->_packages.end(); it++) {
		if (it != this->_packages.begin()) {
			url += "," + *it;
			tmp += ", " + *it;
		} else {
			url = tmp = *it;
		}
	}
	Error::info("Getting info on " + tmp);
	this->_createPackages(this->_getPackagesInfo(url));
}

std::string	Download::_getPackagesInfo(std::string url) {
	CURL		*curl;
	CURLcode	res;
	std::string	result;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, std::string(BASE_URL + std::string("get/") + url).c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Download::_curlCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (res != CURLE_OK) {
			Error::warning(curl_easy_strerror(res));
			Error::error("The request have failed.");
		}
	} else {
		Error::error("Can not initialize curl");
	}
	return result;
}

void	Download::_createPackages(std::string file) {
	Json::Reader	read;
	Json::Value		json;
	Json::ValueIterator		it;

	if (!read.parse(file, json)) {
		Error::warning(read.getFormattedErrorMessages());
		Error::error("The json-response format is bad.");
	}
	this->_response = json;
	for (it = json.begin(); it != json.end(); it++) {
		this->_addPackage(it.key().asString());
	}
}

void	Download::_addPackage(std::string name) {
	Package			*p;
	std::list<std::string>				tmp;
	std::list<std::string>::iterator	it;
	std::list<Package *>::iterator		it2;

	for (it2 = this->_packList.begin(); it2 != this->_packList.end() && (*it2)->getName() != name; it2++);
	if (it2 != this->_packList.end())
		return ;
	p = new Package(name, this->_response[name]);
	if (p->getToInstall()) {
		if (p->getError() == "")
			Error::warning("Package " + name + " already installed, skipping it");
		else
			Error::warning("Package " + name + " can not be found, skipping it");
		delete p;
		return ;
	} else if (!p->getToDownload()) {
		Download::totalSize += p->getSize();
		Download::packCount++;
	} else {
		Error::info("Package " + name + " already downloaded, skipping it");
	}
	if (p->getNeededDeps().size() > 0) {
		tmp = p->getNeededDeps();
		for (it = tmp.begin(); it != tmp.end(); it++)
			this->_addPackage(*it);
	}
	if (p->getRecDeps().size() > 0) {
		tmp = p->getRecDeps();
		for (it = tmp.begin(); it != tmp.end(); it++)
			this->_addPackage(*it);
	}
	this->_packList.push_front(p);
}

size_t	Download::_curlCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void	Download::getAllPackages(void) {
	std::list<Package *>::iterator		it;
	std::string							path, tmp;
	int									res;

	if (Download::packCount) {
		tmp = "Download " + std::to_string(Download::totalSize).substr(0, 3) + "Mo, " + 
			std::to_string(Download::packCount)+ " packages. [Yes/no/details] ";
		while ((res = Error::input(tmp))) {
			if (res == 'Y' || res == 'y' || res == '\n')
				break;
			else if (res == 'n' || res == 'N')
				Error::error("User abort.");
			else if (res == 'd') {
				for (it = this->_packList.begin(); it != this->_packList.end(); it++) {
					Error::info((*it)->getName() + "-" + (*it)->getVersion() + ":\t\t" + 
							std::to_string((*it)->getSize()).substr(0, 3) + "Mo");
				}
				Error::info("Total:\t\t" + std::to_string(Download::totalSize).substr(0, 3) + "Mo");
			} else {
				Error::warning("Please enter [Y]es/[n]o/[d]etails ");
			}
		}
		for (it = this->_packList.begin(); it != this->_packList.end(); it++) {
			path = this->_getArchive((*it)->getUrl());
			(*it)->setArchivePath(path);
			Download::currentCount++;
			Download::currentTotalSize += Download::currentSize;
		}
		std::cout << std::endl;
		Error::success("Download finished !");
	}
}

std::string		Download::_getArchive(std::string url) {
	int				i;
	std::string		path;
	CURL			*curl;
	CURLcode		res;
	FILE			*fd;

	for (i = url.length() - 1; i > 0 && url[i - 1] != '/'; i--);
	path = "/tmp/" + url.substr(i, url.length() - i);
	fd = fopen(path.c_str(), "wb");
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Download::_myFwrite);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, Download::_progress);
		res = curl_easy_perform(curl);
		if (fd)
			fclose(fd);
		if (res != CURLE_OK) {
			std::cout << std::endl;
			Error::warning(curl_easy_strerror(res));
			Error::error("Download failed.");
		}
		curl_easy_cleanup(curl);
	} else {
		Error::error("Can not initialize curl");
	}
	return path;
}

size_t	Download::_myFwrite(void *buffer, size_t size, size_t nmemb, FILE *fd) {
	return fwrite(buffer, size, nmemb, fd);
}

int		Download::_progress(void* ptr, double todl, double nowdl, double toup, double nowup) {
	float	megabytes = nowdl/ 1000 / 1000;

	(void)ptr; (void)todl; (void)toup; (void)nowup;
	Download::currentSize = megabytes;
	std::cout << "\r";
	std::cout << "> Downloading package (" << Download::currentCount << "/" << Download::packCount <<
		"): " << std::setprecision(2) <<  Download::currentSize + Download::currentTotalSize << " / " << Download::totalSize << " Mo                 ";
	return 0;
}

void	Download::installAll(void) {
	std::list<Package *>::iterator		it;
	Exec								*e = new Exec();

	for (it = this->_packList.begin(); it != this->_packList.end(); it++) {
		(*it)->setExec(e);
		(*it)->install();
	}
}

float	Download::totalSize = 0;
float	Download::currentSize = 0;
float	Download::currentTotalSize = 0;
int		Download::packCount = 0;
int		Download::currentCount = 1;
