#include "common.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

int warmUp(int sock, char *buffer, int curr_size_to_send, int type) {
	size_t check = 0;
    for (int i = 0; i < WARMUP_FACTOR; i++)
    {
		if(type) {
			int ret_val = send(sock, buffer, curr_size_to_send, 0);
			if (ret_val < 0){return EXIT_FAILURE;}
		}
        else {
			check = recive_data( sock, buffer, curr_size_to_send);
			if (check < 0) {return EXIT_FAILURE;}
		}

    }
	return 0;
	
}