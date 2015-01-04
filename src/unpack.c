#include "../inc/mpm.h"

static char		*get_extract_directory(void) {
	int		fd = open("/tmp/mpm.log", O_RDONLY);
	char	buffer[50];
	char	**result;

	read(fd, buffer, 50);
	result = ft_strsplit(buffer, '/');
	close(fd);
	remove("/tmp/mpm.log");
	return strjoin("/tmp/", result[0]);
}

extern char **environ;

void	configure(char *path, mpm_pinfo *p) {
	info("Running configure..");
	char	**configure = ft_strsplit(p->configure, ' ');
	int		pid, status;

	configure[0] = strjoin(path, configure[0]);
	if (!(pid = fork())) {
		int		fd = open("/dev/null", O_WRONLY);
		dup2(fd, 1);
		execve(configure[0], configure, environ);
	}
	waitpid(pid, &status, 0);
	if (status > 0)
		error("Configure return an error");
	info("Done.");
}

void	make(mpm_pinfo *p) {
	info("Running make...");
	char	**make = ft_strsplit(p->make, ' ');
	int		pid, status;

	make[0] = strjoin("/usr/bin/", make[0]);
	if (!(pid = fork())) {
		int		fd = open("/dev/null", O_WRONLY);
		dup2(fd, 1);
		execve(make[0], make, environ);
	}
	waitpid(pid, &status, 0);
	if (status > 0)
		error("Make return an error");
	info("Done.");
}

void	make_install(mpm_pinfo *p) {
	info("Install the package...");
	char	**install = ft_strsplit(p->install, ' ');
	int		pid, status;

	install[0] = strjoin("/usr/bin/", install[0]);
	if (!(pid = fork())) {
		int		fd = open("/dev/null", O_WRONLY);
		dup2(fd, 1);
		execve(install[0], install, environ);
	}
	waitpid(pid, &status, 0);
	if (status > 0)
		error("Installation return an error");
	info("Done.");
}

void	unpacking(char *path, mpm_pinfo *p) {
	char	*extract_dir;
	int		pid;

	info("Unpacking ...");
	if (!(pid = fork())){
		int		fd = open("/tmp/mpm.log", O_WRONLY | O_CREAT, 0777);
		dup2(fd, 1);
		execl("/bin/tar", "/bin/tar", "xvf", path, "-C", "/tmp/", NULL);
	}
	waitpid(pid, &pid, 0);
	success("Done.");
	extract_dir = get_extract_directory();
	info("Making build directory");
	mkdir("/tmp/build", 0777);
	info("Changing directory");
	chdir("/tmp/build");
	configure(extract_dir, p);
	make(p);
	make_install(p);
	success("Package installed !");
}
