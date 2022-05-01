#include <stdio.h>     // printf, perror
#include <stdlib.h>    // EXIT_FAILURE, exit
#include <string.h>    // memset, strncmp
#include <unistd.h>    // write, read, close
#include <arpa/inet.h> // connect, socket, htonl, htons, network MACROS
#include "common.h"    // socket_setup

// Function designed for chat between client and server.
void recv_send_msgs(int connfd)
{

    for (size_t j = 0; j < 31; j++)
    {
        uint64_t num_of_bytes_to_send = 1 << j;
        uint8_t *buffer = malloc(num_of_bytes_to_send);

        for (size_t i = 0; i < N_ROUNDS; i++)
        {
            receive_message(num_of_bytes_to_send, connfd, buffer);
        }
        send_message(1, connfd, buffer);

        free(buffer);
    }
}

int bind_listen_accept(int sockfd, struct sockaddr_in *servaddr,
                       socklen_t *client_addr_len, int *connfd, struct sockaddr_in *cli)
{
    // Binding newly created socket to given IP
    if ((bind(sockfd, (const SA *)servaddr, sizeof((*servaddr)))) != EXIT_SUCCESS)
    {
        printf("socket bind failed...\n");
        return EXIT_FAILURE;
    }

    // Now server is ready to listen to a single client
    if ((listen(sockfd, 0)) != EXIT_SUCCESS)
    {
        perror("listen() failed with error no.:\n");
        return EXIT_FAILURE;
    }

    *client_addr_len = sizeof((*cli));
    // Accept the data packet from a client
    *connfd = accept(sockfd, (SA *)cli, client_addr_len);
    if (*connfd < 0)
    {
        perror("accept() failed with error no.:\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main()
{
    const uint16_t port = 8080;

    int sockfd, connfd;
    socklen_t client_addr_len;
    struct sockaddr_in servaddr, cli;

    if (socket_setup(&sockfd, &servaddr, htonl(INADDR_LOOPBACK), port))
    {
        exit(EXIT_FAILURE);
    }

    if (bind_listen_accept(sockfd, &servaddr, &client_addr_len, &connfd, &cli))
    {
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Server is listening...
    set_sockfd_opts(sockfd);

    recv_send_msgs(connfd); // TODO benchmark

    close(sockfd);

    return EXIT_SUCCESS;
}