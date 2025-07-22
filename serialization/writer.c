///usr/bin/env -S gcc -Wall -Wextra -Wpedantic -fanalyzer -Wanalyzer-too-complex -ggdb -g3 -O0 writer.c -o writer; exit $?

#define _GNU_SOURCE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>

// The idea here is that it should be possible to take any datatype and convert
// it into an integer / bytestring that can then be saved to disk and
// reinterpreted from file back into something readable

char* concat(const char * const strs[], int nstrs) {
	// Concatenate all words into a single buffer
	char *end = NULL, *buf = NULL;
	if(asprintf(&end, "%s", strs[0]) < 0)
		error(1, errno, "Could not populate first string");
	for(int i = 1; i < nstrs; i++) {
		if(asprintf(&buf, "%s %s", end, strs[i]) < 0) {
			free(end);
			error(1, errno, "Couldn't append to final buffer");
		}
		free(end);
		end = buf;
	}

	return end;
}

uint32_t* serialize(void *data, size_t size, size_t *reslen) {
	uint32_t *result;
	*reslen = (size / sizeof(*result)) + 1;
	// const size_t ressize = (*reslen) * sizeof(*result);

	result = calloc(*reslen, sizeof(*result));
	if(!result)
		return NULL;

	for(size_t i = 0; i < size; i++)
		*((uint8_t*)result + i) = *((uint8_t*)data + i);

	return result;
}

int main(const int argc, const char * const argv[]) {
	if(argc < 2) {
		fprintf(stderr, "USAGE: %s <word> [word, ...]\nSerializes then writes words to file\n", argv[0]);
		exit(1);
	}

	char *msg = concat(argv + 1, argc - 1);
	size_t msglen = strlen(msg), reslen = 0;
	uint32_t *encoded = serialize(msg, msglen * sizeof(*msg), &reslen);
	free(msg);
	if(!encoded)
		abort();

	for(size_t i = 0; i < reslen; i++)
		printf("%d\n", encoded[i]);

	free(encoded);

	return 0;
}
