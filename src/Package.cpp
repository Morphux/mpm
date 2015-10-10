/*
 * Package.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-07 12:50
 */

# include "Package.hpp"

Package::Package(std::string name, Json::Value init) : _name(name), _toDownload(0), _toInstall(0) {
	Json::ValueIterator		it;
	std::string				path;
	struct stat				buffer;
	int						i;

	path = "/etc/mpm/packages/" + std::string(1, name[0]) + "/" + name;
	if (!(stat(path.c_str(), &buffer)))
		this->_toInstall = 1;
	this->_version = init["version"].asString();
	this->_size = init["size"].asFloat();
	this->_packageUrl = init["package"].asString();
	for (it = init["dependencies"]["needed"].begin(); it != init["dependencies"]["needed"].end(); it++)
		this->_neededDeps.push_back(it->asString());
	for (it = init["dependencies"]["optionnal"].begin(); it != init["dependencies"]["optionnal"].end(); it++)
		this->_optDeps.push_back(it->asString());
	for (i = this->_packageUrl.length() - 1; i > 0 && this->_packageUrl[i - 1] != '/'; i--);
	path = "/tmp/" + this->_packageUrl.substr(i, this->_packageUrl.length() - i);
	if (!(stat(path.c_str(), &buffer))) {
		this->_toDownload = 1;
		this->_archive = path;
	}
}

Package::Package(std::string name) : _name(name), _toInstall(0) {
	std::string				path;
	struct stat				buffer;

	path = "/etc/mpm/packages/" + std::string(1, name[0]) + "/" + name;
	if (!(stat(path.c_str(), &buffer)))
		this->_toInstall = 1;

}

Package::~Package(void) {
	return ;
}

void	Package::install(void) {
	this->decompress();
	this->readPackageInfos();
	if (this->_infos["compilation"]["patches"].size())
		this->patches();
	if (this->_infos["compilation"]["before"].size())
		this->before();
	this->configure();
	this->make();
	this->minstall();
	if (this->_infos["compilation"]["after"].size())
		this->after();
	this->save();
	this->clean();

	Error::success("Installation of " + this->_name + " version " + this->_version + " complete !");
}

void	Package::decompress(void) {
	struct archive			*a, *ext;
	struct archive_entry	*entry;
	int				fd, r, i;
	size_t			size;
	# ifdef __i383__
	long long int		offset;
	# else
	off_t				offset;
	#endif
	const void		*buff;
	std::string		path;

	Error::info("Extracting " + this->_archive);
	a = archive_read_new();
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, ARCHIVE_EXTRACT_TIME);
	archive_read_support_format_all(a);
	archive_read_support_filter_all(a);
	if ((fd = archive_read_open_filename(a, this->_archive.c_str(), 10240)))
		Error::error(std::string(archive_error_string(a)));
	fd = archive_read_next_header(a, &entry);
	for (i = 0, path = "/tmp/"; fd != ARCHIVE_EOF; fd = archive_read_next_header(a, &entry), path = "/tmp/", i++) {
		if (fd != ARCHIVE_OK)
			Error::error(std::string(archive_error_string(a)));
		path += std::string(archive_entry_pathname(entry));
		if (i == 0)
			this->_dir = path;
		archive_entry_set_pathname(entry, path.c_str());
		fd = archive_write_header(ext, entry);
		if (fd != ARCHIVE_OK) {
			Error::warning(std::string(archive_error_string(a)));
		} else {
			for (r = ARCHIVE_OK; r != ARCHIVE_EOF && r == ARCHIVE_OK; r = archive_read_data_block(a, &buff, &size, &offset))
				r = archive_write_data_block(ext, buff, size, offset);
			if (r != ARCHIVE_OK && r != ARCHIVE_EOF)
				Error::warning(std::string(archive_error_string(ext)));
			fd = archive_write_finish_entry(ext);
		}
	}
}

void	Package::readPackageInfos(void) {
	std::string			file;
	std::stringstream 	buffer;
	std::ifstream		fd;
	Json::Reader		read;

	chdir(this->_dir.c_str());
	fd.open("package.json");
	if (!fd.is_open())
		Error::error("The file package.json can't be found in " + this->_dir);
	buffer << fd.rdbuf();
	if (!read.parse(buffer.str(), this->_infos)) {
		Error::warning(read.getFormattedErrorMessages());
		Error::error("The package.json format is bad. Please report it at packages[at]morphux[dot]org");
	}
}

void	Package::patches(void) {
	Error::info("Applying patches");
}

void	Package::before(void) {
	std::string				str;
	std::list<std::string>	args;
	Json::ValueIterator		it;

	Error::info("Executing pre-compilation operations");
	for (it = this->_infos["compilation"]["before"].begin(); it != this->_infos["compilation"]["before"].end(); it++) {
		args = split(it->asString(), ' ');
		this->_exec->execute(args);
		args.clear();
	}
}

void	Package::configure(void) {
	Json::Value		val = this->_infos["compilation"]["configure"];
	Json::ValueIterator		it;
	std::list<std::string>	args;

	chdir("srcs/");
	Error::info("Configuring package");
	for (it = val.begin(); it != val.end(); it++) {
		if (it->isObject())
			args.push_back("--" + it->begin().key().asString() + "=" + it->begin()->asString());
		else
			args.push_back("--" + it->asString());
	}
	args.push_back("--prefix=" + Options::config["installation-directory"].asString());
	args.push_front("configure");
	this->_exec->execute(args);
}

void	Package::make(void) {
	std::list<std::string>				args, opt;
	std::list<std::string>::iterator	it;

	Error::info("Compiling");
	args.push_back("make");
	if (this->_infos["compilation"]["make"].asString() != "") {
		opt = split(this->_infos["compiling"]["make"].asString(), ' ');
		for (it = opt.begin(); it != opt.end(); it++)
			args.push_back(*it);
	}
	this->_exec->execute(args);
}

void	Package::minstall(void) {
	std::list<std::string>				args, opt;
	std::list<std::string>::iterator	it;

	Error::info("Installing");
	args.push_back("make");
	if (this->_infos["compilation"]["install"].asString() != "") {
		opt = split(this->_infos["compilation"]["install"].asString(), ' ');
		for (it = opt.begin(); it != opt.end(); it++)
			args.push_back(*it);
	} else {
		args.push_back("install");
	}
	this->_exec->execute(args);
}


void	Package::after(void) {
	std::string				str;
	std::list<std::string>	args;
	Json::ValueIterator		it;

	Error::info("Executing post-compilation operations");
	for (it = this->_infos["compilation"]["after"].begin(); it != this->_infos["compilation"]["after"].end(); it++) {
		args = split(it->asString(), ' ');
		this->_exec->execute(args);
		args.clear();
	}
}

void	Package::save(void) {
	std::string		str = "/etc/mpm/packages/" + std::string(1, this->_name[0]);
	DIR		*dir = opendir(str.c_str());
	std::stringstream 	buffer;
	std::ifstream		fd;
	std::ofstream		tar;

	Error::info("Saving package");
	if (!dir)
		mkdir(str.c_str(), 0744);
	closedir(dir);
	str += "/" + this->_name;
	dir = opendir(str.c_str());
	if (!dir)
		mkdir(str.c_str(), 0744);
	closedir(dir);
	fd.open(this->_dir + "/srcs/Makefile");
	if (!fd.is_open())
		fd.open(this->_dir + "/srcs/makefile");
	buffer << fd.rdbuf();
	tar.open(str + "/" + "Makefile");
	tar << buffer.str();
	fd.close(); tar.close();
	tar.open(str + "/" + "package.json");
	tar << this->_infos;
	tar.close();
}

void	Package::clean(void) {
	Error::info("Cleaning");
	rmrf(this->_dir.c_str());
	remove(this->_archive.c_str());
}

void	Package::uninstall(void) {
	this->_dir = "/etc/mpm/packages/" + std::string(1, this->_name[0]) + "/" + this->_name;
	std::list<std::string>		args, tmp;
	std::list<std::string>::iterator	it;

	Error::info("Removing " + this->_name);
	args.push_back("make");
	this->readPackageInfos();
	tmp = split(this->_infos["compilation"]["uninstall"].asString(), ' ');
	for (it = tmp.begin(); it != tmp.end(); it++)
		args.push_back(*it);
	this->_exec->execute(args);
	rmrf(this->_dir.c_str());
}

void					Package::setArchivePath(std::string s) { this->_archive = s; };
void					Package::setToDownload(int s) { this->_toDownload = s; };
void					Package::setExec(Exec *e) { this->_exec = e; };

std::string				Package::getVersion(void) { return this->_version; };
std::string				Package::getName(void) { return this->_name; };
std::string				Package::getUrl(void) { return this->_packageUrl; };
std::list<std::string>	Package::getNeededDeps(void) { return this->_neededDeps; };
std::list<std::string>	Package::getOptionnalDeps(void) { return this->_optDeps; };
float					Package::getSize(void) { return this->_size; };
int						Package::getToDownload(void) { return this->_toDownload; };
int						Package::getToInstall(void) { return this->_toInstall; };
