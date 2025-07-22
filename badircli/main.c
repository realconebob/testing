// connect to irc server of choice and send a message, then disconnect.

// Don't care about portability, and I'm using the gnuc library so why not
#define _GNU_SOURCE
#include "main.h"
#include "irc.h"

#include <poll.h>       // poll() for polling the internet socket
#include <error.h>      // error handling with error()
#include <errno.h>      // for use with error()
#include <netdb.h>      // getaddrinfo() for fetching info on a server 
#include <regex.h>      // regexec() for replying to pings
#include <signal.h>     // SIGTERM definition
#include <stdio.h>      // fprintf() for error handling
#include <stdlib.h>     // exit()
#include <string.h>     // memset() and string manipulation
#include <unistd.h>     // Opening and closing files
#include <sys/types.h>  // Types for everything
#include <sys/prctl.h>  // For killing the child process if the parent dies
#include <sys/socket.h> // For actually creating a socket

#include <libdaemon/daemon.h> // libdaemon functions


// Define these here because it isn't "portable"
#define TESTDOMAIN "dabikers.online"
#define IRCPORT "6667"


int main() {
    int status; // love me some error checking

    // Initalize the addrinfo struct
    struct addrinfo *servinfo;
    if((status = init_client_getaddrinfo(TESTDOMAIN, IRCPORT, AF_UNSPEC, SOCK_STREAM, &servinfo)) != 0)
        exit(status);

    // Connect to the desired ip
    int sfd;
    if((status = client_connect_addrinfo(&sfd, servinfo)) != 0)
        exit(status);

    // Free the now useless addrinfo struct
    freeaddrinfo(servinfo);


    struct ircnames name = {.nick="guy",.user="guy",.real="guy head"};

    // Fork to deal with pings and reading from the server
    pingrelay(sfd, name);

    // Join and send a quick message
    irc_connect(sfd, name);
    irc_joinchan(SAMESOCKET, name, "#general");
    irc_privmsg(SAMESOCKET, name, "#general", "testing");

    sleep(1200); // So I can read stuff from the server

    // Server doesn't actually print anything for leave reason, but I think that's my fault and not the program's
    irc_quit(SAMESOCKET, name, "gootbye");

    // Close the connection and free the socket/fd
    cleanup_client(sfd);

    return 0;
}

int pingrelay(int sfd, struct ircnames name) {
    // Fork stuff
    pid_t ppid_before = getpid();
    pid_t listener = fork();
    if(listener < 0)
        error(E_FORK_CANTFORK, errno, "[pingrelay] Couldn't create child process");


    // START OF CHILD PROCESS -/

    if(!listener) { // You'd think this meant the parent, but no this is the child
        int r = prctl(PR_SET_PDEATHSIG, SIGTERM);
        if(r < 0)
            error(E_FORK_CANTWAITOTDIE, errno, "[pingrelay] prctl failed in child process");

        // Check that the parent hasn't already exited somehow
        if(getppid() != ppid_before)
            error(E_FORK_ALREADYDEAD, EOWNERDEAD, "[pingrelay] Parent process already dead");

        // Close extra file descriptors
        if(daemon_close_all(sfd, -1) < 0) {
            fprintf(stderr, "[pingrelay] Failed to close extra file descriptors\n");
            return -1;
        }

        // Set up regex
        regex_t pattern;
        createRegexPattern(&pattern, PINGREGEX, REG_EXTENDED | REG_NOSUB);

        // Set up poll to read from socket
        struct pollfd psfd = {.fd = sfd, .events = POLLIN};

        // Buffer for reading from the socket
        char buff[IRC_MSGMAX];
        memset(buff, 0, IRC_MSGMAX);

        while(1) {
            r = poll(&psfd, sfd, POLL_NOTIMEOUT);
            if(r < 0) {
                error(E_POLL_CANTPOLL, errno, "[pingrelay] Error polling socket");
            } else if(r) { // poll returns a non-zero number if any fd is available, but I'm only checking a single fd, so idc about checking the actual value
                if(recv(sfd, buff, IRC_MSGMAX, 0) <= 0) {
                    printf("[pingrelay] Connection closed by server\n");
                    break;
                }

                printf("%s", buff);

                // Reply if a ping from the server
                if(regexec(&pattern, buff, 0, NULL, 0) == 0) {
                    irc_pong(sfd, name);
                }

                memset(buff, 0, IRC_MSGMAX);
            }
        }
        regfree(&pattern);

        return 0;
    } // END OF CHILD PROCESS -/

    return 0;
}

int createRegexPattern(regex_t *pattern, const char *PATTERN, int flags) {
    int r = regcomp(pattern, PATTERN, flags);
    if(r != 0) {
        char erstr[REGEX_ERRSTR_LEN];
        memset(erstr, 0, REGEX_ERRSTR_LEN);

        regerror(r, NULL, erstr, REGEX_ERRSTR_LEN);
        fprintf(stderr, "[pingrelay] Could not compile regex pattern: %s\n", erstr);
        return -1;
    }

    return 0;
}