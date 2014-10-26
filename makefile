
CC=gcc
CCFLAGS = -g -Wall -I.

DEPS = caps.h
OBJS = caps_server_work.o caps_server_main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

caps: $(OBJS) 
	$(CC) -o $@ $(OBJS) $(CCFLAGS) 


clean:
	rm -rf *.o caps
