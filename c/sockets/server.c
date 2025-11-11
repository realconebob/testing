// Server code
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
    struct sockaddr_in srvsock, clisock;
    int srvfd, clifd;
    int echeck = 0;
    char buf[2048];

    // Create server socket
    srvfd = socket(AF_INET, SOCK_STREAM, 0);
    if(srvfd < 0) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    // Bind server socket to address
    srvsock.sin_family = AF_INET;
    inet_aton("127.0.0.1", &srvsock.sin_addr);
    srvsock.sin_port = htons(9999);
    printf("Address: %s:%d\n", inet_ntoa(srvsock.sin_addr), ntohs(srvsock.sin_port));

    echeck = bind(srvfd, (struct sockaddr*)&srvsock, (socklen_t)sizeof(srvsock));
    if(echeck < 0) {
        perror("Could not bind to address");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    echeck = listen(srvfd, 5);
    if(echeck < 0) {
        perror("Could not listen for incoming connections");
        exit(EXIT_FAILURE);
    }

    // Accept an incoming connection
    int len = sizeof(struct sockaddr_in); // THIS IS NECESSARY FOR SOME  REASON
    int count = 1;

    while(1) {
        clifd = accept(srvfd, (struct sockaddr*)&clisock, (socklen_t*)&len);
        if(clifd < 0) {
            perror("Couldn't accept incoming connection");
            exit(EXIT_FAILURE);
        }

        // Send a string to the client
        memset(buf, 0, 2048);
        snprintf(buf, 2048, "Finally, a connection (Count: %d)", count++);
        echeck = write(clifd, buf, strlen(buf));
        if(echeck < 0) {
            perror("Could not send data to client");
            exit(EXIT_FAILURE);
        }
        
        // Shutdown after sending data
        shutdown(clifd, SHUT_RD);
    }
    return 0;
}