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
    for (size_t j = 0; j < 31; j++)
    {
        // Init buffers
        uint64_t num_of_bytes_to_send = 1 << j;
        uint8_t *rbuffer = malloc(num_of_bytes_to_send);
        uint8_t *wbuffer = malloc(num_of_bytes_to_send);
        clock_t tsend, tstart;

        tstart = clock();
        for (size_t i = 0; i < N_ROUNDS; i++)
        {
            send_message(num_of_bytes_to_send, sockfd, wbuffer);
        }
        tsend = clock();
        receive_message(1, sockfd, rbuffer);

        clock_t send_time = (tsend - tstart) / CLOCKS_PER_SEC;
        clock_t throughput = (N_ROUNDS * num_of_bytes_to_send) / send_time;

        printf("%lu\t%lu\tSeconds\n", num_of_bytes_to_send, throughput);

        free(rbuffer);
        free(wbuffer);
    }
}

int main()
{
    const char *server_host = "127.0.0.1";

    int sockfd;
    struct sockaddr_in servaddr;

    // set client's socket up
    if (socket_setup(&sockfd, &servaddr, inet_addr(server_host), PORT))
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

    apply_benchmark(sockfd);

    close(sockfd);
    return EXIT_SUCCESS;
}