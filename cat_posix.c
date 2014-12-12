#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SZ 4096

static void cat(int fd, int *err)
{
	char buf[BUF_SZ];
	size_t n;
	while (1) {
		if ((n = read(fd, buf, sizeof(buf))) == -1) {
			perror("cat: read");
			*err = EXIT_FAILURE;
			return;
		}
		if (n == 0) {
			/* EOF reached, successfully read the whole file */
			return;
		}
		if (write(STDOUT_FILENO, buf, n) == -1) {
			perror("cat: write");	
			*err = EXIT_FAILURE;
			return;
		}
	}
}

int main(int argc, char **argv)
{
	int err = EXIT_SUCCESS, fd;
	if (argc == 1) {
		cat(STDIN_FILENO, &err);
	}
	for (argv++; *argv; argv++) {
		if (!strcmp(*argv, "-")) {
			cat(STDIN_FILENO, &err);
		}
		else {
			if ((fd = open(*argv, O_RDONLY)) == -1) {
				perror("cat: open");
				err = EXIT_FAILURE;
				continue;
			}
			cat(fd, &err);
			if (close(fd) == -1) {
				perror("cat: close");
				err = EXIT_FAILURE;
				continue;
			}
		}
	}
	return err;
}
