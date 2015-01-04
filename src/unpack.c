#include "../inc/mpm.h"

static char		*get_extract_directory(void) {
	int		fd = open("/tmp/mpm.log", O_RDONLY);
	char	buffer[50];
	char	**result;

	read(fd, buffer, 50);
	result = ft_strsplit(buffer, '/');
	close(fd);
	return strjoin("/tmp/", result[0]);
}

extern char **environ;

void	exec(char *cmd, int type, char *prefix) {
	char	**command = ft_strsplit(cmd, ' ');
	int		pid, status, fd;

	if (type == 0)
		info("Running configure...");
	else if (type == 1)
		info("Running make...");
	else if (type == 2)
		info("Running make install...");
	command[0] = strjoin(prefix, command[0]);
	if (!(pid = fork())) {
		fd = open("/var/log/mpm.log", O_WRONLY | O_APPEND | O_CREAT, 0777);
		dup2(fd, 1);
		dup2(fd, 2);
		execve(command[0], command, environ);
	}
	waitpid(pid, &status, 0);
	close(fd);
	if (status > 0)
		error("An error append, see /var/log/mpm.log for more information");
	printf("Done.\n");
}

static void		do_extract(char *path) {
	int		pid, fd;

	info("Unpacking...");
	if (!(pid = fork())){
		fd = open("/tmp/mpm.log", O_WRONLY | O_CREAT | O_TRUNC, 0777);
		dup2(fd, 1);
		execl("/bin/tar", "/bin/tar", "xvf", path, "-C", "/tmp/", NULL);
	}
	waitpid(pid, &pid, 0);
	close(fd);
	success("Done.");
}

static void		package_config(mpm_pinfo *p) {
	int		fd, fd2;
	char	buffer[1];

	info("Making local configuration");
	mkdir(strjoin("/etc/mpm/", p->name), 0777);
	fd = open("/tmp/build/Makefile", O_RDONLY);
	fd2 = open(strjoin("/etc/mpm/", strjoin(p->name, "/Makefile")), O_CREAT | O_WRONLY , 0777);
	if ((fd == -1)) {
		info("Can't find /tmp/build/Makefile, trying to find /tmp/build/makefile");
		close(fd);
		fd = open("/tmp/build/makefile", O_RDONLY);
		if (fd == -1)
			error("Failed. No makefile found. (The Package is installed but can't be uninstall). Please mail to packages[at]lists.morphux.org about this situation");
	} else if (fd2 == -1)
		error("Can't open the config directory. Is the chmod of /etc/mpm good (777)?");
	while (read(fd, buffer, 1))
		write(fd2, buffer, 1);
	printf("Done.\n");
}

static void		clean(mpm_pinfo *p, char *extract) {
	info("Cleaning...");
	exec(strjoin("rm -rf /tmp/build ", extract), 3, "/bin/");
	remove("/tmp/mpm.log");
}

void	unpack_install(char *path, mpm_pinfo *p) {
	char	*extract_dir;

	do_extract(path);
	extract_dir = get_extract_directory();
	info("Making build directory");
	mkdir("/tmp/build", 0777);
	info("Changing directory");
	chdir("/tmp/build");
	exec(p->configure, 0, extract_dir);
	exec(p->make, 1, "/usr/bin/");
	exec(p->install, 2, "/usr/bin/");
	package_config(p);
	clean(p, extract_dir);
	success("Package installed !");
}
