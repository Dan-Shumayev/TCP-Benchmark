#include <stdio.h>     // printf, perror
#include <stdlib.h>    // EXIT_FAILURE, exit
#include <string.h>    // memset, strncmp
#include <unistd.h>    // write, read, close
#include <arpa/inet.h> // connect, socket, htonl, htons, network MACROS
#include "common.h"    // socket_setup, MACROS
#include <time.h>      // clock, CLOCKS_PER_SEC
#include <inttypes.h>  // PRIu64

void apply_benchmark(int sockfd)
{
    // Timed send-receive loop
    uint64_t *times_send = malloc(sizeof(uint64_t) * 31);
    uint64_t *times_recv = malloc(sizeof(uint64_t) * 31);

    for (size_t j = 0; j < 31; j++)
    {
        // Init buffers
        uint64_t num_of_bytes_to_send = 1 << j;
        uint8_t *rbuffer = malloc(num_of_bytes_to_send);
        uint8_t *wbuffer = malloc(num_of_bytes_to_send);
        uint64_t tsend, tstart;

        tstart = clock();
        for (size_t i = 0; i < N_ROUNDS; i++)
        {
            send_message(num_of_bytes_to_send, sockfd, wbuffer);
        }
        tsend = clock();
        receive_message(1, sockfd, rbuffer);

        uint64_t tend = clock();
        times_send[j] = tsend - tstart;
        times_recv[j] = tend - tsend;

        printf("%lu, %lu\n", times_send[j], times_recv[j]);
    }
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
    set_sockfd_opts(sockfd);

    apply_benchmark(sockfd); // TODO - benchmark stuff

    close(sockfd);
    return EXIT_SUCCESS;
}