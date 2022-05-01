#ifndef COOMON_H
#define COMMON_H

#include <arpa/inet.h>   // connect, socket, htonl, htons, network MACROS
#include <stdio.h>       // printf, perror
#include <stdlib.h>      // EXIT_FAILURE, exit
#include <string.h>      // memset, strncmp
#include <fcntl.h>       // fcntl
#include <unistd.h>      // fcntl
#include <sys/types.h>   // setsockopt
#include <sys/socket.h>  // setsockopt
#include <netinet/tcp.h> // TCP_NODELAY

#define SA struct sockaddr
#define N_ROUNDS 1000000

/** Setting up a new socket */
int socket_setup(int *sockfd, struct sockaddr_in *servaddr, in_addr_t sock_address,
                 in_port_t sock_port);

int set_sockfd_opts(int sockfd);

int receive_message(size_t n_bytes, int sockfd, uint8_t *buffer);

int send_message(size_t n_bytes, int sockfd, uint8_t *buffer);

#endif