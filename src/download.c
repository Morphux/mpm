#include "../inc/mpm.h"

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
	struct FtpFile *out = (struct FtpFile *)stream;
	if (out && !out->stream) {
		out->stream=fopen(out->filename, "wb");
		if(!out->stream)
			return -1;
	}
	return fwrite(buffer, size, nmemb, out->stream);
}

static char		*getTarName(char *url) {
	char	**tmp = ft_strsplit(url, '/');
	int		i;

	for (i = 0; tmp[i]; i++);
	return strjoin("/tmp/", tmp[i - 1]);
}

static int	progress_func(void* ptr, double TotalToDownload, double NowDownloaded, 
	double TotalToUpload, double NowUploaded) {

	int		totaldotz = 60;
	double	fractiondownloaded = NowDownloaded / TotalToDownload;
	int		dotz = round(fractiondownloaded * totaldotz);
	int		ii = 0;

	printf("%3.0f%% [",fractiondownloaded*100);
	for (; ii < dotz; ii++) {
		if (ii == dotz - 1)
			printf(">");
		else
			printf("=");
	}
	for (; ii < totaldotz; ii++) {
		printf(" ");
	}
	printf("]   \r");
	fflush(stdout);
	return 0; 
}

void	download(mpm_pinfo *p) {
	CURL		*curl;
	CURLcode	res;
	struct FtpFile ftpfile = {
		getTarName(p->url),
		NULL
	};
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	while (p) {
		info(strjoin("Downloading ", p->name));
		printf("=> %s\n", getTarName(p->url));
		curl_easy_setopt(curl, CURLOPT_URL, p->url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
		res = curl_easy_perform(curl);
		printf("\n");
		if (ftpfile.stream)
			fclose(ftpfile.stream);
		if (CURLE_OK != res) {
		printf(">> %d\n", res);
			error("Download failed");
		}
		curl_easy_cleanup(curl);
		fflush(stdout);
		printf("\nDone!\n");
		unpack_install(getTarName(p->url), p);
		p = p->next;
	}
	curl_global_cleanup();
}
