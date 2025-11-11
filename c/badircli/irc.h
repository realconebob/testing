#ifndef _IRC_H_12112023
#define _IRC_H_12112023

// Error macros (negative so I can test 'r < 0')
#define E_GAI_CANTGETINFO -1
#define E_SOCKET_CANTOPEN -2
#define E_SOCKET_CANTCONNECT -3
#define E_SOCKET_CANTCLOSE -4
#define E_IRC_MTOOBIG -5
#define E_MEM_CANTALLOC -6
#define E_IRC_NOMSG -7
#define E_FORK_CANTFORK -8
#define E_FORK_CANTCLOSE -9
#define E_FORK_CANTWAITOTDIE -10
#define E_FORK_ALREADYDEAD -11
#define E_FORK_NOPIPE -12
#define E_IRC_NONAMES -13
#define E_IRC_NOFD -14
#define E_POLL_CANTPOLL -15

// Value for using the same socket in functions that take a socket
#define SAMESOCKET -1

// IRC Messages are limited to lengths of 512
#define IRC_MSGMAX (sizeof(char) * 512)
#define IRC_CRLF_LEN (sizeof(char) * 2)
#define IRC_CRLF "\x0D\x0A" // equivalent to \r\n


// Used for starting and ending a capabilities exchange / startup connection with an irc server
#define IRC_CONNMSGSTART "CAP LS\x0D\x0A"
#define IRC_CONNMSGEND "CAP END\x0D\x0A"

#define IRC_NICK_MAXLEN 30 // I can't find an RFC that sets this in stone
struct ircnames {
    char nick[IRC_NICK_MAXLEN];
    char user[IRC_NICK_MAXLEN];
    char real[IRC_NICK_MAXLEN];
};

// The poll() function will time out if the var timeout is a non-negative number
#define POLL_NOTIMEOUT -1

#include <netdb.h>

// calloc() wrapper that aborts the program if it can't allocate memory. Same arguments as a normal calloc call
void* xcalloc(size_t nmemb, size_t size);

// Function used to deal with partial sends, written by Beej (https://beej.us/guide/bgnet/html/#sendall)
int sendall(int socket, char *buf, int *len, int flags);

// Initialize an addrinfo* struct using PF for protocol family, and SOCKTYPE for the type of socket. returns non-zero on error
int init_client_getaddrinfo(const char *DOMAIN, const char *PORT, int pf, int socktype, struct addrinfo **RES);

// Connect to a remote server using an addrinfo struct. sfd becomes the server's socket, returns non-zero on error
int client_connect_addrinfo(int *sfd, struct addrinfo *res);

// Close connection and socket SFD
int cleanup_client(int sfd);


// wrapper for vsnprintf(), returns a string of length <= 512 bytes allocated on the heap. Returns null on error
char* _irc_makemsg(char *format, ...);

// Send "CAP LS", "NICK", "USER", and "CAP END" commands to server
int irc_connect(int sfd, struct ircnames names);

// Join channel 'chan' on server
int irc_joinchan(int sfd, struct ircnames names, const char *chan);

// Send a PRIVMSG of 'msg' to 'recip'
int irc_privmsg(int sfd, struct ircnames names, const char *recip, const char *msg);

// Send a QUIT message to the server
int irc_quit(int sfd, struct ircnames names, const char *reason);

// Reply to pings from server
int irc_pong(int sfd, struct ircnames names);

#endif