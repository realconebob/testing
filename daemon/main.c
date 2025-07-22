#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#define P_READ 0
#define P_WRITE 1

static void damonize() {
	pid_t pid;

	// Fork once
	pid = fork();
	if(pid < 0) {
		fprintf(stderr, "Error forking child process\n");
		exit(EXIT_FAILURE);
	}
	// Exit the parent process
	if(pid > 0)
		exit(EXIT_SUCCESS);
	
	// set sid of child
	if(setsid() < 0)
		exit(EXIT_FAILURE);

	// Ignore signals (implement signal handler later)
	signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

	// Fork again
	pid = fork();
	if(pid < 0) {
		fprintf(stderr, "Error forking child process\n");
		exit(EXIT_FAILURE);
	}
	// Exit the parent process
	if(pid > 0)
		exit(EXIT_SUCCESS);

	// Daemon setup
	umask(117); // Set rwx permissions to ug=rw,o=
	chdir("/"); // Move to a directory that will always exist
	for(int i = sysconf(_SC_OPEN_MAX); i >= 0; i--) { // Close all open file descriptors
		close(i);
	}
	openlog("Test Daemon", LOG_PID, LOG_USER); // Open a log file

	return;
}

int main() {
	// Become a daemon
	damonize();

	// Test code
	int x = 0;
	while(1) {
		syslog(LOG_USER | LOG_INFO, "Yo, does this work? Iter: %d\n", x++);
		sleep(20);
	}

	syslog(LOG_AUTH | LOG_NOTICE, "Test daemon halted\n");
	closelog();

	return 0;
}