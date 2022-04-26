#include <stdio.h>     // printf, perror
#include <stdlib.h>    // EXIT_FAILURE, exit
#include <string.h>    // memset, strncmp
#include <unistd.h>    // write, read, close
#include <arpa/inet.h> // connect, socket, htonl, htons, network MACROS
#include "common.h"    // socket_setup, MACROS

void apply_benchmark(int sockfd)
{
    (void)sockfd;
}

int main()
{
    const uint16_t port = 8080;
    const char *server_host = "127.0.0.1";

    int sockfd;
    struct sockaddr_in servaddr;

    // set client's socket up
    if (socket_setup(&sockfd, &servaddr, inet_addr(server_host), port))
    {
        exit(EXIT_FAILURE);
    }

    // connect the client socket to server socket
    if (connect(sockfd, (const SA *)&servaddr, sizeof(servaddr)))
    {
        perror("connect() failed with error no:\n");

        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // connected to the server...
    apply_benchmark(sockfd); // TODO - benchmark stuff

    close(sockfd);
    return EXIT_SUCCESS;
}