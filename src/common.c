#include "common.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>     

int socket_setup(int *sockfd, struct sockaddr_in *servaddr, in_addr_t sock_address, in_port_t sock_port)
{
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1)
    {
        perror("socket() failed with error no:\n");
        return EXIT_FAILURE;
    }

    memset(servaddr, 0, sizeof((*servaddr))); // Clean up the socket address's space

    // Set the socket fields up
    servaddr->sin_family = AF_INET;
    servaddr->sin_addr.s_addr = sock_address;
    servaddr->sin_port = htons(sock_port);

    return EXIT_SUCCESS;
}

int set_sockfd_opts(int sockfd)
{
    // Turn on the Non-Blocking bit of the socket file descriptors's flags
    // such that read() and write() operations would never block
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("fcntl() failed with error no.:\n");
        return EXIT_FAILURE;
    }

    int flag = 1;

    // Disable the Nagle's algorithm executing under the TCP protcol, such that each sent
    // data is immediately gets on the wire without further ado
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(int)) == -1)
    {
        perror("setsockopt() failed with error no.:\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Reads from the given socket into the given buffer n_bytes bytes
int receive_message(size_t n_bytes, int sockfd, uint8_t *buffer)
{
    size_t bytes_read = 0;
    int r;

    while (bytes_read < n_bytes)
    {
        // Make sure we read exactly n_bytes
        r = read(sockfd, buffer, n_bytes - bytes_read);
        if (r < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK))
        {
            perror("read() failed with error no.:\n");
            exit(EXIT_FAILURE);
        }

        if (r > 0)
        {
            bytes_read += r;
        }
    }

    return bytes_read;
}

// Writes n_bytes from the given buffer to the given socekt
int send_message(size_t n_bytes, int sockfd, uint8_t *buffer)
{
    size_t bytes_sent = 0;
    int r;

    while (bytes_sent < n_bytes)
    {
        // Make sure we write exactly n_bytes
        r = write(sockfd, buffer, n_bytes - bytes_sent);

        if (r < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK))
        {
            perror("write() failed:");
            exit(EXIT_FAILURE);
        }

        if (r > 0)
        {
            bytes_sent += r;
        }
    }

    return bytes_sent;
}