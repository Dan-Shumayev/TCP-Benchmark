#ifndef COOMON_H
#define COMMON_H

#include <arpa/inet.h> // connect, socket, htonl, htons, network MACROS
#include <stdio.h>     // printf, perror
#include <stdlib.h>    // EXIT_FAILURE, exit
#include <string.h>    // memset, strncmp

#define SA struct sockaddr

/** Setting up a new socket */
int socket_setup(int *sockfd, struct sockaddr_in *servaddr, in_addr_t sock_address, 
                                                                    in_port_t sock_port);

#endif