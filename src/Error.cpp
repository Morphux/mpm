/*
 * Error.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 06:09
 */

#include "Error.hpp"

void	Error::info(std::string s) {
	std::cout << PURP << "> " << DEF << s << std::endl;
}

void	Error::warning(std::string s) {
	std::cerr << YELLOW << "Warning: " << DEF << s << std::endl;
}

void	Error::error(std::string s) {
	std::cerr << RED << "Error: " << DEF << s << std::endl;
	_exit(1);
}

void	Error::success(std::string s) {
	std::cout << GREEN << "Success: " << DEF << s << std::endl;
}

void	Error::log(std::string s) {
	int		fd;
	fd = open("/var/log/mpm.log", O_WRONLY | O_APPEND | O_CREAT, 0444);
	write(fd, s.c_str(), s.length());
	close(fd);
}

int		Error::input(std::string s) {
	char	buff[1024];

	s = BLUE + std::string("? ") + DEF + s;
	write(1, s.c_str(), s.length());
	read(0, buff, 1024);
	return buff[0];
}

void	Error::execError(void) {
	int						r;
	std::list<std::string>	lines;
	std::list<std::string>::iterator	it;
	std::stringstream 	buffer;
	std::ifstream		fd;

	fd.open("/var/log/mpm.log");
	if (!fd.is_open())
		return ;
	buffer << fd.rdbuf();
	lines = split(buffer.str(), '\n');
	for (it = lines.end(), r = 0; it != lines.begin() && r < 20 && (*it).substr(0, 6) != "======"; it--, r++);
	if (r == 20)
		Error::info("Output the last 20 lines. If the information is not here, you might check /var/log/mpm.log");
	else
		Error::info("Ouput");
	for (; it != lines.end(); it++)
		std::cout << RED << "> " << DEF << *it << std::endl;
}

std::string		Error::programName = "";
int				Error::debugFlag = 0;
