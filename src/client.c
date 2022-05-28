#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"





struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec-start.tv_sec -1;
        temp.tv_nsec = 1000000000 + end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec ;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}
// use ?
void warm_up(int socket,uint64_t num_of_bytes_to_send)
{
	uint8_t *tempBuffer = malloc(num_of_bytes_to_send);
	for (int i = 0; i < N_ROUNDS; ++i)
	{
		send_message(socket, tempBuffer, num_of_bytes_to_send);
	}
}

void apply_benchmark(int socket)
{
	for (size_t j = 0; j < NUM_OF_BITS; j++) {
		uint64_t num_of_bytes_to_send = 1 << j;
		uint8_t *rbuffer = malloc(num_of_bytes_to_send);
		uint8_t *wbuffer = malloc(num_of_bytes_to_send);

		struct timespec start, end ,res;

		clock_gettime(CLOCK_REALTIME, &start);
		for (size_t i = 0; i <  N_ROUNDS; i++) {

			send_message(num_of_bytes_to_send, socket,wbuffer);
		}
		clock_gettime(CLOCK_REALTIME, &end);
		res = diff(start,end);
		double send_time = ((double) res.tv_sec + ((double) res.tv_nsec / BILLION));
		double throughput = (((float) num_of_bytes_to_send / MB) * N_ROUNDS) / send_time;
		
		fprintf(stdout, "%lu\t%f\tMbps\n", num_of_bytes_to_send, throughput);

	}
}


int main(int argc, char const* argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	struct addrinfo addrinfo;
	addrinfo.ai_protocol = IPPROTO_TCP;
	addrinfo.ai_family = AF_INET;
	addrinfo.ai_socktype = SOCK_STREAM;
	

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	addrinfo.ai_addr = (struct sockaddr*)&serv_addr;

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	apply_benchmark(sock);

	//endregion
	
}




