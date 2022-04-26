#include <stdio.h>     // printf, perror
#include <stdlib.h>    // EXIT_FAILURE, exit
#include <string.h>    // bzero, strncmp
#include <unistd.h>    // write, read, close
#include <arpa/inet.h> // connect, socket, htonl, htons, network MACROS

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int connfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;)
    {
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // and send that buffer to client
        write(connfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd;
    socklen_t client_addr_len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket() failed with error no.:n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket successfully created...\n");
    }

    bzero(&servaddr, sizeof(servaddr)); // Clean up the socket address's space
    // Set the socket fields up
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* TODO - Listen only to local address connections
                                                        To be changed upon receiving school machines */
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP
    if ((bind(sockfd, (const SA *)&servaddr, sizeof(servaddr))) != EXIT_SUCCESS)
    {
        printf("socket bind failed...\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket successfully binded..\n");
    }

    // Now server is ready to listen to a single client
    if ((listen(sockfd, 0)) != EXIT_SUCCESS)
    {
        perror("listen() failed with error no.:\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Server listening...\n");
    }

    client_addr_len = sizeof(cli);
    // Accept the data packet from a client
    connfd = accept(sockfd, (SA *)&cli, &client_addr_len);
    if (connfd < 0)
    {
        perror("accept() failed with error no.:\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("server accept the client...\n");
    }

    func(connfd); // TODO benchmark stuff?

    close(sockfd);
    return EXIT_SUCCESS;
}