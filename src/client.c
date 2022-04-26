#include <stdio.h>  // printf, perror
#include <stdlib.h> // EXIT_FAILURE, exit
#include <string.h> // bzero, strncmp
#include <unistd.h> // write, read, close
#include <arpa/inet.h> // connect, socket, htonl, htons, network MACROS

#define MAX 80
#define SA struct sockaddr

const uint16_t port = 8080;
const char *sever_host = "127.0.0.1";

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\n");
            return;
        }
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket() failed with error no:\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket successfully created...\n");
    }

    bzero(&servaddr, sizeof(servaddr)); // Clean up the socket address's space
    // Set the socket fields up
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(sever_host);
    servaddr.sin_port = htons(port);

    // connect the client socket to server socket
    if (connect(sockfd, (const SA *)&servaddr, sizeof(servaddr)) != EXIT_SUCCESS)
    {
        perror("connect() failed with error no:\n");

        close(sockfd);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("connected to the server...\n");
    }

    func(sockfd); // TODO - benchmark?

    close(sockfd);
    return EXIT_SUCCESS;
}