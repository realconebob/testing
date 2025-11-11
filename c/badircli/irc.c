#define _GNU_SOURCE
#include "irc.h"

// Sockets and dealing with internet
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>

// Error handling
#include <error.h>
#include <errno.h>

// Other stuff
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

// Signal handling for killing the child process
#include <sys/prctl.h>
#include <signal.h>

// For _irc_makemsg
#include <stdarg.h>

// Regex for replying to pings
#include <regex.h>



void* xcalloc(size_t nmemb, size_t size) {
    void *mem = calloc(nmemb, size);
    if(mem == NULL) {
        error(0, errno, "[xcalloc] Virtual memory exhausted");
        abort();
    }

    return mem;
}


// Function for handling partial sends. (Thanks Beej [https://beej.us/guide/bgnet/html/#sendall])
int sendall(int socket, char *buf, int *len, int flags) {
    // I don't want to specify the socket every time, so now I don't need to
    static int holdover = -1;
    if(socket >= 0)
        holdover = socket;

    if(holdover < 0) {
        error(0, ENOTSOCK, "[sendall] No socket specified");
        return -1;
    }

    int total = 0; // Total bytes sent so far
    int bytesleft = *len; // Number of bytes that need to be sent
    int n = -1;

    while(total < *len) {
        n = send(holdover, buf+total, bytesleft, flags);
        if(n < 0) {break;}
        total += n;
        bytesleft -= n;
    }

    *len = total;

    return (n == -1) ? -1 : 0;

    // This function is pretty slick, especially the buf+total
}

int init_client_getaddrinfo(const char *DOMAIN, const char *PORT, int pf, int socktype, struct addrinfo **RES) {
    struct addrinfo hint;
    int status = 0;

    // set up the hint struct for getaddrinfo
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = pf;
    hint.ai_socktype = socktype;

    // get information on the specified address
    if((status = getaddrinfo(DOMAIN, PORT, &hint, RES)) != 0) {
        fprintf(stderr, "[init_client_getaddrinfo] Couldn't get server info: %s\n", gai_strerror(status));
        return E_GAI_CANTGETINFO;
    }

    return 0;
}

int client_connect_addrinfo(int *sfd, struct addrinfo *res) {
    struct addrinfo *p = NULL;

    // Loop through the linked-list and connect to the first valid address
    for(p = res; p != NULL; p = p->ai_next) {
        // Create a socket
        if((*sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            error(0, errno, "[client_connect_addrinfo]: Could not create socket");
            continue;
        }

        // Connect to the socket
        if(connect(*sfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(*sfd);
            error(0, errno, "[client_connect_addrinfo] Could not connect to socket");
            continue;
        }

        break;
    }

    // Make sure the socket exists and is connected to
    if(p == NULL) {
        error(0, errno, "[client_connect_addrinfo] Couldn't create socket");
        return E_SOCKET_CANTOPEN;
    }

    return 0;
}

int cleanup_client(int sfd) {
    // Shut the connection down
    if(shutdown(sfd, SHUT_RD) < 0) {
        error(0, errno, "[cleanup_client] Couldn't shutdown remote connection");
        return E_SOCKET_CANTCLOSE;
    }

    // Close the file descriptor used by the socket
    if(close(sfd) < 0) {
        error(0, errno, "[cleanup_client] Couldn't close socket");
        return E_SOCKET_CANTCLOSE;
    }

    return 0;
}

char* _irc_makemsg(char *format, ...) {
    // Make sure there's a format
    const int LOCALSIZE = IRC_MSGMAX + 1; // +1 because of vsnprintf's null terminating byte '\0'
    if(format == NULL) {
        fprintf(stderr, "[_irc_makemsg] 'char *format' is null\n");
        return NULL;
    }

    va_list ap;
    va_start(ap, format);
    char tmp[LOCALSIZE];
    memset(tmp, 0, LOCALSIZE);

    // Do vsnprintf
    int r = vsnprintf(tmp, LOCALSIZE, format, ap);
    va_end(ap);

    // Make sure vsnprintf didn't break
    if(r < 0) {
        fprintf(stderr, "[_irc_makemsg] error running vsnprintf\n");
        return NULL;
    } else if(r >= LOCALSIZE) {
        fprintf(stderr, "[_irc_makemsg] message too long, %ld bytes too many (%d/%ld)\n", (r - (IRC_MSGMAX)), r, (IRC_MSGMAX));
        return NULL;
    }

    // Copy the result of vsnprintf into msg
    char *msg = strdup(tmp);
    if(msg == NULL) {
        error(0, errno, "[_irc_makemsg] Could not duplicate tmp string into msg");
        return NULL;
    }

    return msg;
}


int irc_connect(int sfd, struct ircnames names) {
    // To connect, the user must send a set of commands
        // CAP LS
        // NICK
        // USER
        // CAP END

    // This function takes care of all 4 commands in a single shot

    if(names.nick == NULL || names.user == NULL || names.real == NULL) {
        fprintf(stderr, "[irc_connect] Nickname, Username, or Real Name left blank\n");
        return -1;
    }

    char *msg;
    for(int i = 0; i < 4; i++) {
        switch(i) {
            case 0:
                msg = _irc_makemsg(IRC_CONNMSGSTART);
                break;
            case 1:
                msg = _irc_makemsg("NICK %s\x0D\x0A", names.nick);
                break;
            case 2:
                msg = _irc_makemsg("USER %s 0 * :%s\x0D\x0A", names.user, names.real);
                break;
            case 3:
                msg = _irc_makemsg(IRC_CONNMSGEND);
                break;
        }
        if(msg == NULL) {
            fprintf(stderr, "[irc_connect] Could not format message %d for connecting", i);
            return -1;
        }

        printf("[Client] SENDING: %s", msg);

        int len = strlen(msg);
        if(sendall(sfd, msg, &len, 0) < 0) {
            free(msg);
            error(0, errno, "[irc_connect] Error sending message \"%s\" to server", msg);
            return -1;
        }
        free(msg);
    }

    return 0;
}

int irc_joinchan(int sfd, struct ircnames names, const char *chan) {
    char *msg = _irc_makemsg(":%s JOIN %s\x0D\x0A", names.nick, chan);
    if(msg == NULL) {
        fprintf(stderr, "[irc_joinchan] Could not format string for joining a channel\n");
        return -1;
    }

    printf("[Client] SENDING: %s", msg);

    int len = strlen(msg);
    if(sendall(sfd, msg, &len, 0) < 0) {
        free(msg);
        error(0, errno, "[irc_joinchan] Could not send message \"%s\" to server", msg);
        return -1;
    }

    free(msg);

    return 0;
}

int irc_privmsg(int sfd, struct ircnames names, const char *recip, const char *msg) {
    // :guy PRIVMSG #general :text
    char *server_msg = _irc_makemsg(":%s PRIVMSG %s :%s\x0D\x0A", names.nick, recip, msg);
    if(server_msg == NULL) {
        fprintf(stderr, "[irc_joinchan] Could not format string to send PRIVMSG\n");
        return -1;
    }

    printf("[Client] SENDING: %s", server_msg);

    int len = strlen(server_msg);
    if(sendall(sfd, server_msg, &len, 0) < 0) {
        free(server_msg);
        error(0, errno, "[irc_privmsg] Could not send message \"%s\" to server", server_msg);
        return -1;
    }

    free(server_msg);

    return 0;
}

int irc_quit(int sfd, struct ircnames names, const char *reason) {
    // :guy QUIT :Quit: gootbye
    char *msg = _irc_makemsg(":%s QUIT :Quit: %s\x0D\x0A", names.nick, reason);
    if(msg == NULL) {
        fprintf(stderr, "[irc_quit] Could not format message to quit server\n");
        return -1;
    }

    printf("[Client] SENDING: %s", msg);

    int len = strlen(msg);
    if(sendall(sfd, msg, &len, 0) < 0) {
        free(msg);
        error(0, errno, "[irc_quit] Could not send message \"%s\" to server", msg);
        return -1;
    }

    free(msg);

    return 0;
}

int irc_pong(int sfd, struct ircnames names) {
    // PONG
    // lol

    char *msg = _irc_makemsg("PONG :%s\x0D\x0A", names.nick);
    if(msg == NULL) {
        fprintf(stderr, "[irc_pong] Could not format message to reply to ping\n");
        return -1;
    }

    printf("[Client] SENDING: %s", msg);

    int len = strlen(msg);
    if(sendall(sfd, msg, &len, 0) < 0) {
        free(msg);
        error(0, errno, "[irc_pong] Could not send message \"%s\" to server", msg);
        return -1;
    }

    free(msg);

    return 0;
}