#ifndef CAPS_H
#define CAPS_H


#define MSGLEN 30
#define PORT_NUM 4444
#define BACKLOG 5

void run_client_handler(int client_fd);
void run_client(int client_fd);


#endif
