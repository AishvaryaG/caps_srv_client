#include "caps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_client_handler(int client_fd)
{
	printf("Client thread. client_fd: %d", client_fd);
	char str[MSGLEN];
	snprintf(str, MSGLEN, "Hello Chaitanya & Aishvarya!!!");
	send(client_fd, str, strlen(str), 0);	
}

