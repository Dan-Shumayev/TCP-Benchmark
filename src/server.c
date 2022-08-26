#include <stdio.h>
#include<sys/time.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h>



size_t recive_data(int sockfd, char *buffer, int size) {
    size_t bytes_received = 0;
    size_t result = 0;
    while (bytes_received < size)
    {
        result = recv(sockfd, buffer + bytes_received, size - bytes_received, 0);
        if (result == 0)
        {
            printf("Error: recive_data\n");
        }
        else if (result < 0)
        {
            // socket was closed on remote end or hit an error
            // either way, the socket is likely dead
            return result;
        }
        else
        {
            bytes_received += result;
        }
    }

    return result;
}

int connectS()
{
	int socket_desc, client_sock, c;
	struct sockaddr_in server, client;


	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Error: socket\n");
		return EXIT_FAILURE;
	}


	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// Bind
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		printf("Error: bind");
		return EXIT_FAILURE;
	}

	listen(socket_desc, 3);


	printf("Waiting for connection . . .\n");

	c = sizeof(struct sockaddr_in);

	client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c);
	if (client_sock < 0)
	{
		printf("Error: accept");
		return EXIT_FAILURE;
	}

	close(socket_desc);

	return client_sock;
}

int main(int argc, char *argv[])
{
	int sockfd = 0;
	char buffer[MESSAGE_SIZE];
	int curr_size_to_send = 1;
	size_t out_val = 0;


	sockfd = connectS();
    if (sockfd == EXIT_FAILURE) {return EXIT_FAILURE;}

	printf("Connected\n");

	for (int j = 0; j <= EXP_LIMIT; j++)
	{
        int warmVal = warmUp(sockfd, buffer, curr_size_to_send, 0);
        if (warmVal < 0) {
				printf("Error: warmUp");
				return EXIT_FAILURE;
			}


		for (int i = 0; i < MSG_AMOUNT; i++)
		{

			out_val = recive_data(sockfd, buffer, curr_size_to_send);

			if (out_val < 0)
			{
				printf("Error: recive_data");
				return EXIT_FAILURE;
			}
		}
		out_val = send(sockfd, buffer, curr_size_to_send, 0);
		if (out_val < 0)
		{
			printf("Error: send");
			return EXIT_FAILURE;
		}

		curr_size_to_send *= 2;
	}

	close(sockfd);

	return EXIT_SUCCESS;
}