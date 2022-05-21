#include <stdio.h>     // printf, perror
#include <stdlib.h>    // EXIT_FAILURE, exit
#include <string.h>    // memset, strncmp
#include <unistd.h>    // write, read, close
#include <arpa/inet.h> // connect, socket, htonl, htons, network MACROS
#include "common.h"    // socket_setup, MACROS
#include <time.h>      // clock, CLOCKS_PER_SEC
#include <inttypes.h>  // PRIu64





struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec-start.tv_sec -1;
        temp.tv_nsec = 1000000000 + end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec ;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

void apply_benchmark(int sockfd)
{
    // Timed send-receive loop
    for (size_t j = 0; j < NUM_OF_BITS; j++)
    {
        // Init buffers
        uint64_t num_of_bytes_to_send = 1 << j;
        uint8_t *rbuffer = malloc(num_of_bytes_to_send);
        uint8_t *wbuffer = malloc(num_of_bytes_to_send);

        struct timespec start, end, res;

        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);

      
        for (size_t i = 0; i < N_ROUNDS; i++)
        {
            send_message(num_of_bytes_to_send, sockfd, wbuffer);
        }
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
       
        res = diff(start,end);
        receive_message(1, sockfd, rbuffer);


        double send_time = ((double) res.tv_sec + ((double) res.tv_nsec / BILLION)) ; // in second
        double throughput =( (N_ROUNDS * num_of_bytes_to_send) / send_time) / TRANS_MBPS; // Mbps

        printf("%lu\t%f\t Mbps\n",num_of_bytes_to_send, throughput);
        
    
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
