#include "common.h"

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

    set_sockfd_opts(*sockfd);

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