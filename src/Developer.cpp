/*
 * Developer.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-13-12 19:17
 */

#include "Developer.hpp"

Developer::Developer(void) {
	return ;
}

Developer::~Developer(void) {
	return ;
}

void	Developer::createPackage(std::list<std::string> lst) {
	std::list<std::string>::iterator	it = lst.begin();
	std::string	path, archive, name = *it++, url = *it;
	std::ofstream	file;

	mkdir(name.c_str(), 0775);
	chdir(name.c_str());
	path = this->getArchive(url);
	archive = this->extract(path);
	rename(archive.c_str(), "srcs");
	file.open("package.json");
	file << BASIC_FILE << std::endl;
	file.close();
	remove(path.c_str());
}

std::string	Developer::getArchive(std::string url) {
	int				i;
	std::string		path;
	CURL			*curl;
	CURLcode		res;
	FILE			*fd;

	for (i = url.length() - 1; i > 0 && url[i - 1] != '/'; i--);
	path = url.substr(i, url.length() - i);
	fd = fopen(path.c_str(), "wb");
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
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

std::string	Developer::extract(std::string archive) {
	struct archive			*a, *ext;
	struct archive_entry	*entry;
	int						fd, r, i;
	size_t					size;
	# ifdef __i386__
	long long int			offset;
	# else
	off_t					offset;
	#endif
	const void				*buff;
	std::string				path, dir;

	Error::info("Extracting " + archive);
	a = archive_read_new();
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, ARCHIVE_EXTRACT_TIME);
	archive_read_support_format_all(a);
	archive_read_support_filter_all(a);
	if ((fd = archive_read_open_filename(a, archive.c_str(), 10240)))
		Error::error(std::string(archive_error_string(a)));
	fd = archive_read_next_header(a, &entry);
	for (i = 0, path = "./"; fd != ARCHIVE_EOF; fd = archive_read_next_header(a, &entry), path = "./", i++) {
		if (fd != ARCHIVE_OK)
			Error::error(std::string(archive_error_string(a)));
		path += std::string(archive_entry_pathname(entry));
		if (i == 0)
			dir = path;
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
	return dir;
}
