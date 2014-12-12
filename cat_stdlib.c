#include <stdio.h>
#include <stdlib.h>

#define BUF_SZ 4096

static void cat(FILE *f, int *err)
{
	char buf[BUF_SZ];
	size_t n;
	while (1) {
		n = fread(buf, 1, sizeof(buf), f);
		if (n == 0 && ferror(f)) {
			perror("cat: fread");
			*err = EXIT_FAILURE;
			return;
		}
		if (n == 0 && feof(f)) {
			/* EOF reached, successfully read the whole file */
			return;
		}
		if (fwrite(buf, 1, n, stdout) < n) {
			perror("cat: fwrite");	
			*err = EXIT_FAILURE;
			return;
		}
	}
}

int main(int argc, char **argv)
{
	int err = EXIT_SUCCESS;
	FILE *f;
	if (argc == 1) {
		cat(stdin, &err);
	}
	for (argv++; *argv; argv++) {
		if (!strcmp(*argv, "-")) {
			cat(stdin, &err);
		}
		else {
			if ((f = fopen(*argv, "rb")) == NULL) {
				perror("cat: fopen");
				err = EXIT_FAILURE;
				continue;
			}
			cat(f, &err);
			if (fclose(f)) {
				perror("cat: fclose");
				err = EXIT_FAILURE;
				continue;
			}
		}
	}
	return err;
}
