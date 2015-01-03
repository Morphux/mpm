#include "../inc/mpm.h"

void	init_string(struct string *s) {
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	if (s->ptr == NULL)
		error("malloc failed");
	s->ptr[0] = '\0';
}

size_t		writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
	size_t new_len = s->len + size*nmemb;
	s->ptr = realloc(s->ptr, new_len+1);
	if (s->ptr == NULL)
		error("realloc failed");
	memcpy(s->ptr+s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;
	return size*nmemb;
}

static char			*get_web_page(char *name) {
	CURL		*curl;
	CURLcode	res;
	curl = curl_easy_init();
	if (curl) {
		struct string s;
		init_string(&s);

		curl_easy_setopt(curl, CURLOPT_URL, strjoin(HOST_URL, name));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		res = curl_easy_perform(curl);
		if (s.ptr[0] == 0)
			error("Could not find the host website " HOST_URL);
		else if (!strcmp(s.ptr, "Error"))
			error("The package can't be found");
		curl_easy_cleanup(curl);
		return s.ptr;
	} else {
		error("Could not load CURL");
	}
}

mpm_pinfo	*get_info(char *name, mpm_pinfo *packages) {
	char	*conf;

	info(strjoin("Getting information about ", name));
	conf = get_web_page(name);
	packages = parse(conf, packages);
	return packages;
}

void	install(mpm_package *p) {
	mpm_pinfo	*packages;
	while (p) {
		packages = get_info(p->package, packages);
		p = p->next;
	}
	print_packages(packages);
}
