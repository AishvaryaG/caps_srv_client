#include "caps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void run_client(int client_fd)
{
	int num_bytes;
	printf("Client thread. client_fd: %d", client_fd);
	char str[MSGLEN];
	num_bytes = recv(client_fd, str, MSGLEN - 1, 0);
	if (num_bytes == -1) {
		perror("recv");
		exit(1);
	}
	
	str[num_bytes] = '\0';
	
	printf("Msg received: %s", str);

}



