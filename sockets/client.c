// Client code
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main() {
    struct sockaddr_in srvsock;
    int srvfd, msglen, echeck = 0;
    char buf[2048];

    // Create a socket & define the address to connect to
    srvfd = socket(AF_INET, SOCK_STREAM, 0);
    if(srvfd < 0) {
        perror("Could not initalize socket");
        exit(EXIT_FAILURE);
    }

    srvsock.sin_family = AF_INET;
    inet_aton("127.0.0.1", &srvsock.sin_addr);
    srvsock.sin_port = htons(9999);
    printf("Trying to connect to: %s:%d\n", inet_ntoa(srvsock.sin_addr), ntohs(srvsock.sin_port));

    // Connect to socket
    echeck = connect(srvfd, (struct sockaddr*)&srvsock, (socklen_t)sizeof(srvsock));
    if(echeck < 0) {
        perror("Could not connect to remote host");
        exit(EXIT_FAILURE);
    }

    // Receive message from server
    memset(buf, 0, 2048);
    msglen = read(srvfd, buf, 2047);
    if(msglen < 0) {
        perror("Could not read message from server");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", buf);

    shutdown(srvfd, SHUT_RD);
    return 0;
}