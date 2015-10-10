/*
 * Exec.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-09 02:26
 */

# include "Exec.hpp"

extern char		**environ;

Exec::Exec(void) {
	this->_path = split(std::string(getenv("PATH")), ':');
	// Need to add this by-hand, for local binaries
	this->_path.push_back(".");
}

Exec::~Exec(void) {
	return ;
}

void	Exec::execute(std::list<std::string> args) {
	struct stat				buffer;
	const char				**execArguments;
	int						pid, status, fd;
	std::list<std::string>::iterator	it;

	for (it = this->_path.begin(); it != this->_path.end(); it++) {
		if (!(stat(std::string(*it + "/" + args.front()).c_str(), &buffer)))
			break ;
	}
	if (it == this->_path.end())
		Error::error("The program " + args.front() + " can't be found.");
	*(args.begin()) = *it + "/" + args.front();
	execArguments = this->_listToArray(args);
	fd = open("/var/log/mpm.log", O_WRONLY | O_APPEND | O_CREAT, 0444);
	Error::log("========================[ " + args.front() + " ]========================\n");
	if (!(pid = fork())) {
		dup2(fd, 1);
		dup2(fd, 2);
		// Yup, this is nasty. Why ? Thank's to const standard. (http://stackoverflow.com/questions/190184/execv-and-const-ness)
		execve(execArguments[0], const_cast<char * const *>(execArguments), const_cast<char * const *>(environ));
	} 
	waitpid(pid, &status, 0);
	close(fd);
	if (status) {
		Error::warning("Execution of " + args.front() + " went wrong.");
		Error::execError();
		Error::error("Execution error");
	}
}

const char	**Exec::_listToArray(std::list<std::string> args) {
	const char	**ret;
	int		i;
	std::list<std::string>::iterator	it;

	ret = (const char **)malloc(sizeof(char *) * args.size() + 1);
	for (i = 0, it = args.begin(); (size_t)i < args.size(); i++, it++)
		ret[i] = (*it).c_str();
	ret[i] = NULL;
	return ret;
}
