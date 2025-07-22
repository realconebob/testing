#include <threads.h>
#include <stdio.h>

struct jej {
	const char * const string;
	const int iter;
};

int callback(void *data) {
	if(!data)
		return -1;

	struct jej real = *(struct jej *)data;
	printf("%s %d\n", real.string, real.iter);
	return 0;
}

int main() {
	thrd_t threads[100];
	for(int i = 0; i < (sizeof(threads) / sizeof(threads[0])); i++) {
		thrd_create(&threads[i], callback, (void*)(&(struct jej){.string = "this is some text", .iter = i}));
		thrd_join(threads[i], NULL);
	}

	return 0;
}

// So yes it is possible to print from a thread, which means my thing isn't working
