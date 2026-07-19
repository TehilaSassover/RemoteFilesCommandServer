#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include "../../common/constants/constants.h"

int create_server_socket(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("Failed to create socket");
        return -1;
    }

    printf("Server socket file descriptor: %d\n", server_fd);

    struct sockaddr_in address = {0};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd,
             (struct sockaddr *)&address,
             sizeof(address)) == -1)
    {
        perror("Failed to bind socket");
        close(server_fd);
        return -1;
    }

    printf("Server bound to address and port\n");

    int backlog = SERVER_BACKLOG;

    if (listen(server_fd, backlog) == -1)
    {
        perror("Failed to listen for connections");
        close(server_fd);
        return -1;
    }

    printf("Server listening for connections...\n");

    return server_fd;
}

int accept_client(int server_fd)
{
    int client_fd = accept(server_fd,
                           NULL,
                           NULL);

    if (client_fd == -1)
    {
        perror("Failed to accept connection");
        return -1;
    }

    printf("Accepted a connection from client.\n");

    return client_fd;
}
