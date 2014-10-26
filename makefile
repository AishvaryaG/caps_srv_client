
CC=gcc
CCFLAGS = -g -Wall -I.

DEPS = caps.h
SERVER_OBJS = caps_server_work.o caps_server_main.o
CLIENT_OBJS = caps_client_work.o caps_client_main.o

all: caps_server caps_client

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

caps_server: $(SERVER_OBJS) 
	$(CC) -o $@ $(SERVER_OBJS) $(CCFLAGS) 


caps_client: $(CLIENT_OBJS) 
	$(CC) -o $@ $(CLIENT_OBJS) $(CCFLAGS) 


clean:
	rm -rf *.o caps_client caps_server
