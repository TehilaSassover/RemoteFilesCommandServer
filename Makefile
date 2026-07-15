CC = gcc
CFLAGS = -Wall -Wextra

COMMON = common/constants/constants.c \
         common/validation/validation.c

CLIENT_SRC = client/client.c $(COMMON)
SERVER_SRC = server/server.c $(COMMON)

.PHONY: all client server clean

all: client server

client:
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o client/client

server:
	$(CC) $(CFLAGS) $(SERVER_SRC) -o server/server

clean:
	rm -f client/client server/server