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

    return EXIT_SUCCESS;
}