#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common.h"


void warm_up(int new_socket, const char *buffer, int iter, int data)
{
	u_long dataRec = 0, dataExc = iter * data;
	while (dataRec < dataExc)
	{
		int valread = recv(new_socket, buffer, data,0);
		if(valread > 0){
			dataRec += valread;
		}
	}
}

void recv_data(int new_socket, const char *buffer, int dataNeeded)
{
	u_long iters = 1;
	u_long dataRec = 0;
	u_long dataExc = iters * dataNeeded;
	while (dataRec < dataExc)
	{
		int valread = recv(new_socket, buffer, dataNeeded,0);
		if(valread > 0){
			dataRec += valread;
		}
	}
	send(new_socket, buffer, 1, 0);
}

void recv_send_msg(int connfd) {
	for (size_t j = 0; j < NUM_OF_BITS; j ++) {
		uint64_t num_of_bytes_to_send = 1 << j;
		uint8_t *sbuffer = malloc(num_of_bytes_to_send);
		for (size_t i = 0; i < N_ROUNDS; i++) {
			receive_message(1, connfd, sbuffer);
		}
		send_message(1, connfd, sbuffer);
		free(sbuffer);
	}
}


int main(int argc, char const* argv[])
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	char buffer[MB] = { 0 };

	// region open server socket

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}


	if (setsockopt(server_fd, SOL_SOCKET,
				   SO_REUSEADDR, &opt, // had | OS_REUSEPORT
				   sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	//recv_send_msg(new_socket);

	for (int i = 1; i <= MB; i *= 2)
		{
			warm_up(new_socket, buffer, 1500, i);
			recv_data(new_socket, buffer, i);
		}

		printf("Done\n");
		return 0;
}

