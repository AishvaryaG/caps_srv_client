
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "caps.h"

// Returns server socket_fd
int setup_server(int *p_sock_fd);

void sigchld_handler(int s); 
void sigint_handler(int s);
void init_signal_handler();

int main (int argc, char *argv[])
{

	int client_fd;
	int server_fd;
	int ret;

	init_signal_handler();	

	// client data
	struct sockaddr addr;
	socklen_t addr_len;

	ret = setup_server(&server_fd);
	if (ret != 0) {
		perror("Unable to setup server!");
		exit(1);
	}


	while(1) {
		client_fd = accept(server_fd, &addr, &addr_len);
		if (-1 == client_fd) {
			perror("Unable to accept");
			exit(1);
		}

		if (!fork()) { //child process
			close(server_fd); //let go of the server fd in child thread

			run_client_handler(client_fd);
			close(client_fd);
			exit(0);
		} else {
			// parent process doesn't need this
			close (client_fd);
		}		
	}

	return 0;
}


void sigchld_handler(int s) 
{		
	// Waits for any process (-1) with no static location	
	while(waitpid(-1, NULL, WNOHANG) > 0); 
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
	sa.sa_flags = SA_RESTART;

	sa.sa_handler = sigchld_handler;

	if (-1 == sigaction(SIGCHLD, &sa, NULL)) {
		perror("sigaction sigchld");
		exit(1);
	}	

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sigint_handler;
	if (-1 == sigaction(SIGINT, &sa, NULL)) {
		perror("sigaction sigint");
		exit(1);
	}	
	
}

int setup_server(int *p_sock_fd)
{

	int ret, len;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; //open to both ipv4 and v6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	char port_num[10];

	len = snprintf(port_num, 10, "%d", PORT_NUM);
	port_num[len] = '\0';

	ret = getaddrinfo(NULL , port_num, &hints, &res);
	if (ret != 0) {
		fprintf(stderr, "Unable to lookup: %s", gai_strerror(ret));
		goto cleanup;
	}	 

	*p_sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	//printf("sock_fd: %d", *p_sock_fd);
	
	if (*p_sock_fd == 0) {
		fprintf(stderr, "Unable to create_socket");
		ret = -1;
		goto cleanup;
	}

	ret = bind(*p_sock_fd, res->ai_addr, res->ai_addrlen);
	if (ret != 0) {
		fprintf(stderr, "Unable to bind socket: %s", gai_strerror(ret));
		goto cleanup;
	}	 

	ret = listen(*p_sock_fd, BACKLOG);
	if (ret != 0) {
		perror("Unable to listen");
		goto cleanup;
	}

	ret = 0;

cleanup:
	freeaddrinfo(res);
	return ret;
}


