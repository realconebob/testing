#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "You need to add a string dummy\n");
		return 1;
	}

	int pipes[2];
	if(pipe(pipes)) {
		fprintf(stderr, "no plumbing :(\n");
		return 1;
	}
	pid_t pid = fork();
	if(pid == -1) {
		fprintf(stderr, "no fork :(\n");
		return 1;
	}

	write(pipes[PIPE_WRITE], argv[1], 2048);

	if(pid == 0) {
		dup2(fileno(stdout), pipes[PIPE_WRITE]);
		dup2(pipes[PIPE_READ], fileno(stdin));
		execl("/bin/ls", "ls", NULL);
	}

	char msg[2048];
	if(pid != 0) {
		read(pipes[PIPE_READ], msg, 2048);
		printf("%s\n", msg);
	}

	return 0;
}
