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




struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec-start.tv_sec -1;
        temp.tv_nsec = BILLION + end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec ;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}


int connectC(char *argv[])
{
	int sockfd;
	struct sockaddr_in server;

	//Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Error: socket\n");
		return EXIT_FAILURE;
	}

	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	if (connect(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		perror("Error: connect");
		return EXIT_FAILURE;
	}

	return sockfd;
}

size_t recive_data(int sockfd, char *buffer, int size)
{

size_t bytes_received = 0;
size_t res = 0;
while (bytes_received < size)
{
    res = recv(sockfd, buffer + bytes_received, size - bytes_received, 0);
    if (res == 0)
    {
        printf("Error: recv\n");
    }
    else if (res < 0)
    {

        return res;
    }
    else
    {
        bytes_received += res;
    }
}

return res;
}


int main(int argc, char *argv[])
{
	int sockfd = 0;
	char buffer[MESSAGE_SIZE];
	int num_of_bytes_to_send = 1;  
	size_t out_val = 0;


	sockfd = connectC(argv);

	if (sockfd == EXIT_FAILURE){return EXIT_FAILURE;}

	printf("Connect!\n");

	for (int j = 0; j <= EXP_LIMIT; j++)
	{
        int check = warmUp(sockfd, buffer, num_of_bytes_to_send, 1);
        if (check < 0) {
				printf("Error: warmUp");
				return EXIT_FAILURE;
			}

		struct timespec start, end, res;

		clock_gettime(CLOCK_MONOTONIC, &start);

		for (int i = 0; i < MSG_AMOUNT; i++)
		{
			out_val = send(sockfd, buffer, num_of_bytes_to_send, 0);
			if (out_val < 0)
			{
				printf("Error: send");
				return EXIT_FAILURE;
			}
		}
		clock_gettime(CLOCK_MONOTONIC, &end);

		res = diff(start, end);

		double send_time = (((double) res.tv_sec * BILLION + ((double) res.tv_nsec)) / MSG_AMOUNT) *1e-9; // in second's
        double throughput =((double) (EIGHT * num_of_bytes_to_send) / (MB)) / send_time; // M'bps

        printf("%d\t%f\t Mbps\n",num_of_bytes_to_send, throughput);
		out_val = recive_data(sockfd, buffer, num_of_bytes_to_send);
		if (out_val < 0)
		{
			printf("Error: recive_data");
			return EXIT_FAILURE;
		}

		num_of_bytes_to_send *= 2;
	}

	close(sockfd);

	return EXIT_SUCCESS;
}