/*
 * Options.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 05:59
 */


#include "Options.hpp"

void	Options::addOptions(std::string opt) {
	Options::map[opt] = "";
}

void	Options::addOptions(std::string opt, std::string val) {
	Options::map[opt] = val;
}

bool	Options::isOptions(std::string s) {
	if (Options::map.find(s) != Options::map.end())
		return true;
	return false;
}

std::string		Options::getOptions(std::string s) {
	std::map<std::string, std::string>::iterator	it;

	it = Options::map.find(s);
	if (it == Options::map.end())
		return "";
	return it->second;
}

void	Options::getConfig(void) {
	std::string			file;
	std::stringstream 	buffer;
	std::ifstream		fd;
	Json::Reader		read;

	fd.open("/etc/mpm/config.json");
	if (!fd.is_open())
		Error::error("The configuration file can not be found");
	buffer << fd.rdbuf();
	if (!read.parse(buffer.str(), Options::config)) {
		Error::warning(read.getFormattedErrorMessages());
		Error::error("The configuration file is bad formatted");
	}
}

std::map<std::string, std::string>	Options::map = {};
std::list<std::string>				Options::args = {};
Json::Value							Options::config = false;
