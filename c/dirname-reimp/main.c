#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void *xmalloc(size_t size) {
	void *mem = malloc(size);
	if(!mem)
		abort();

	return mem;
}

char *xdirname(const char * const path) {
	char *tmp = NULL;
	if(!path) { // Path being null is a special case which should return super early, before anything else
		tmp = strdup(".");
		if(!tmp)
			abort();

		return tmp;
	}

	unsigned char flag = 0;
	if(strcmp(path, ".") 	== 0) 			{tmp = strdup("."); flag++;}
	if(strcmp(path, "/") 	== 0 && !flag) 	{tmp = strdup("/"); flag++;}
	if(strcmp(path, "..") 	== 0 && !flag) 	{tmp = strdup("."); flag++;}

	if(flag) {
		if(!tmp)
			abort();

		return tmp;
	}


	/* From the manpages: (man 3 dirname)
	//		+=======================================+
	//		|	path		dirname		basename	|
	//		+=======================================+
	//		|	/usr/lib   	/usr      	lib			|
	//		|	/usr/      	/         	usr			|
	//		|	usr        	.         	usr			|
	//		+=======================================+
	*/

	// Get a temp copy of the path for manipulation purposes
	tmp = strdup(path);
	if(!tmp)
		abort();

	// If there's a trailing '/', delete it
	size_t pathlen = strlen(path);
	if(tmp[pathlen - 1] == '/') {
		tmp[pathlen - 1] = '\0';
		pathlen--;
	}

	// Ok, I think the easiest way to do this (if maybe a bit slow) is to count the number of '/'s in the string
		// If there's only one, return '/'
		// If there are 2 or more, find the last one in the list and set it to '\0'

	size_t count = 0;
	for(size_t i = 0; i < pathlen; i++) {
		if(tmp[i] == '/')
			count++;
	}

	if(count == 0) {
		free(tmp);
		tmp = strdup(".");
		if(!tmp)
			abort();

		return tmp;
	}
	if(count == 1) {
		free(tmp);
		tmp = strdup("/");
		if(!tmp)
			abort();

		return tmp;
	}

	for(size_t i = 0, c2 = 0; i < pathlen; i++) {
		if(tmp[i] == '/')
			c2++;
		if(c2 == count)
			tmp[i] = '\0';
	}

	char * const actual = strdup(tmp);
	if(!actual)
		abort();
	free(tmp);

	return actual;
}

int basicTest(void) {
	char *tmp = NULL;

	tmp = xdirname(NULL);
	if(strcmp(tmp, ".") != 0)
		abort();
	free(tmp);

	tmp = xdirname(".");
	if(strcmp(tmp, ".") != 0)
		abort();
	free(tmp);

	tmp = xdirname("..");
	if(strcmp(tmp, ".") != 0)
		abort();
	free(tmp);

	tmp = xdirname("/");
	if(strcmp(tmp, "/") != 0)
		abort();
	free(tmp);

	return 0;
}

int main() {
	basicTest();

	char *test = NULL;
	test = xdirname("usr");
	if(!test)
		abort();
	printf("%s\n", test);
	free(test);

	test = xdirname("/usr/");
	if(!test)
		abort();
	printf("%s\n", test);
	free(test);

	test = xdirname("/usr/lib");
	if(!test)
		abort();
	printf("%s\n", test);
	free(test);

	test = xdirname("/usr/lib/");
	if(!test)
		abort();
	printf("%s\n", test);
	free(test);

	return 0;
}

// Alright, this is a feature complete copy of dirname, except it allocates a string as the return and aborts on malloc error