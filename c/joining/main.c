#include <threads.h>
#include <stdio.h>
#include <unistd.h>

static int testfunc(void *data) {
	data = data;
	printf("This is a test function\n");
	return 0;
}

int main(void) {
	thrd_t thread;
	thrd_create(&thread, testfunc, NULL);
	sleep(10);
	thrd_join(thread, NULL);
	printf("Joined after thread exited\n");
	return 0;
}
