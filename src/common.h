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


#define EIGHT 8
#define BILLION 1000000000L

#define PORT 8080
#define MB 1048576

#define WARMUP_FACTOR 150 
										
#define MSG_AMOUNT 10000		
										
#define EXP_LIMIT 10
#define MESSAGE_SIZE 2000


size_t recive_data(int sock, char *buffer, int size);

int warmUp(int sock, char *buffer, int curr_size_to_send, int type);

#endif