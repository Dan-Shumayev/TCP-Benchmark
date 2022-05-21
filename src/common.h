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
#include <time.h>      // clock, CLOCKS_PER_SEC

#define SA struct sockaddr
#define N_ROUNDS 50
#define NUM_OF_BITS 11
#define BILLION 1000000000L
#define TRANS_MBPS 125000

#define PORT 25463


/** Setting up a new socket */
int socket_setup(int *sockfd, struct sockaddr_in *servaddr, in_addr_t sock_address,
                 in_port_t sock_port);

int set_sockfd_opts(int sockfd);

int receive_message(size_t n_bytes, int sockfd, uint8_t *buffer);

int send_message(size_t n_bytes, int sockfd, uint8_t *buffer);

#endif