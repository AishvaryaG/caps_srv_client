#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "caps.h"

// Returns server socket_fd
int setup_client(int *p_sock_fd);

void sigchld_handler(int s); 
void sigint_handler(int s);
void init_signal_handler();

int main (int argc, char *argv[])
{

	int sock_fd;
	int ret;

	init_signal_handler();	

	ret = setup_client(&sock_fd);
	if (ret != 0) {
		perror("Unable to setup server!");
		exit(1);
	}

	run_client(sock_fd);
	close(sock_fd);

	return 0;
}


void sigint_handler(int s)
{
	printf("Thanks for running the server. Exiting...\n");
	exit(0);
}

void init_signal_handler()
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sigint_handler;
	if (-1 == sigaction(SIGINT, &sa, NULL)) {
		perror("sigaction sigint");
		exit(1);
	}	
	
}

int setup_client(int *p_sock_fd)
{

	int ret;
	struct addrinfo hints, *res, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; //open to both ipv4 and v6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	char port_num[6];
	snprintf(port_num, 6, "%d", PORT_NUM);

	ret = getaddrinfo(NULL, port_num, &hints, &res);
	if (ret != 0) {
		fprintf(stderr, "Unable to lookup: %s", gai_strerror(ret));
		goto cleanup;
	}	 

	for (p = res; p->ai_next != NULL; p = p->ai_next) {
		*p_sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		//printf("sock_fd: %d", *p_sock_fd);
		
		if (*p_sock_fd == 0) {
			fprintf(stderr, "Unable to create_socket");
			ret = -1;
			goto cleanup;
		}

		ret = connect(*p_sock_fd, p->ai_addr, p->ai_addrlen);
		if (ret != 0) {
			perror("Unable to listen");
			goto cleanup;
		}
		break;
		ret = 0;
	}

cleanup:
	freeaddrinfo(res);
	return ret;
}


