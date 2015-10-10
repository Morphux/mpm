/*
 * functions.cpp
 * By: Louis Solofrizzo <louis@ne02ptzero.me>
 * 2015-10-10 05:03
 */

# include "functions.hpp"

std::list<std::string>		split(std::string s, char delim) {
	std::stringstream		ss(s);
	std::string				item;
	std::list<std::string>	ret;

	while (std::getline(ss, item, delim)) {
		if (!item.empty())
			ret.push_back(item);
	}
	return ret;
}

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
	(void)sb; (void)typeflag; (void)ftwbuf;
	return remove(fpath);
}

int rmrf(const char *path) {
	return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}
