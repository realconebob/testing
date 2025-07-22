/*
Client code
Compile it with: gcc -Wall -Wextra -Werror client.c -o client
by Marco Cetica <ceticamarco@gmail.com> 2021
https://marcocetica.com/posts/socket_tutorial_part1/
*/

#include <stdio.h> // printf, puts
#include <string.h> // strlen
#include <stdlib.h> // atoi, memset
#include <unistd.h> // close

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main() {
    int server_fd = 0, ret = 0;
    struct sockaddr_in server_sock;
    char msg[BUF_SIZE], server_msg[BUF_SIZE];

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd  < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    // Setup socket and buffers
    //server_sock.sin_addr.s_addr = inet_addr(argv[1]);
    inet_aton("127.0.0.1", &server_sock.sin_addr);
    server_sock.sin_port = htons(9999);
    server_sock.sin_family = AF_INET;
    memset(server_msg, 0, sizeof(server_msg));
    sprintf(msg, "Hello World");


    // 2. Connect to server
    ret = connect(server_fd, (struct sockaddr*)&server_sock, sizeof(server_sock));
    if(ret < 0) {
        puts("Unable to connect to remote host");
        return 1;
    }

    // 3. Send data to server
    ret = send(server_fd, msg, strlen(msg), 0);
    if(ret < 0) {
        puts("Unable to send data to remote server");
        return 1;
    }

    // Read server's response
    ret = recv(server_fd, server_msg, sizeof(server_msg), 0);
    if(ret < 0) {
        puts("Unable to read data from remote server");
        return 1;
    }

    printf("%s", server_msg);


    // Finally, close socket
    close(server_fd);
    
    return 0;
}