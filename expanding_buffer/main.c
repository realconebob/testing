#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>

void* xcalloc(size_t nmemb, size_t size) {
	void *mem = calloc(nmemb, size);
	if(!mem)
		error(1, errno, "Could not allocate %ldx%ld (%ld) bytes, quitting...", nmemb, size, nmemb * size);

	return mem;
}

int readwholebuffer(char **str, unsigned long int initsize, int fd) {
	// Try to read bytes from fd into str
		// Bytes read == 0, return 0
		// Bytes read < 0, free string, return -1;
	// When string hits capacity, double the capacity, and reallocate the string

	char *lstr = NULL, *tmp = NULL;
	ssize_t bytesread = -1;
	unsigned long csize = initsize, ccap = initsize;

	lstr = xcalloc(initsize, sizeof(char));
	while((bytesread = read(fd, lstr + (csize - ccap), ccap)) > 0) {
		ccap -= bytesread;
		if(ccap <= 0) {
			csize *= 2;
			ccap = csize / 2;

			tmp = realloc(lstr, csize * sizeof(char));
			if(!tmp) {
				error(0, errno, "Could not reallocate enough space for the string");
				free(lstr);
				bytesread = -100;
				break;
			}
			lstr = tmp;
		}

	}
	if(bytesread < 0 && bytesread != -100) {
		error(0, errno, "Ran into a read() error");
		free(lstr);
		lstr = NULL;
	}

	if(lstr) {
		tmp = realloc(lstr, strlen(lstr) + 1);
		if(!tmp) {
			error(0, errno, "Could not shrink lstr after reading buffer");
			free(lstr);
			bytesread = -100;
		}
		lstr = tmp;
	}

	*str = lstr;
	return (bytesread == 0) ? 0 : -1;
}

int main() {
	char *str = NULL;
	if(readwholebuffer(&str, 20, STDIN_FILENO) < 0)
		error(1, 0, "Could not read buffer for some reason, quitting...");

	printf("Read in from stdin: %s\n", (str != NULL) ? str : "<Could not read from stdin>");
	free(str);

	return 0;
}
