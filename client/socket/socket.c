#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include "../../common/constants/constants.h"
#include "../../common/types/types.h"
#include "../../common/validation/validation.h"


int connect_to_server(const char *ip, int port)
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1)
    {
        perror("Failed to create socket");
        return -1;
    }

    printf("Client socket file descriptor: %d\n", client_fd);

    struct sockaddr_in server_address = {0};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    inet_pton(AF_INET, ip, &server_address.sin_addr);

    if (connect(client_fd,
                (struct sockaddr *)&server_address,
                sizeof(server_address)) == -1)
    {
        perror("Failed to connect to server");
        close(client_fd);
        return -1;
    }

    printf("Client connected to server\n");

    return client_fd;
}



void send_command(int socket_fd, Command *command)
{
    char buffer[MAX_BUFFER_SIZE];

    int path_length = strlen(command->path);
    int content_length = (command->content != NULL) ? strlen(command->content) : 0;

    snprintf(buffer,
             MAX_BUFFER_SIZE,
             "%d" SEPARATOR_STRING
             "%d" SEPARATOR_STRING
             "%d" SEPARATOR_STRING
             "%s%s",
             command->operation,
             path_length,
             content_length,
             command->path,
             (command->content != NULL) ? command->content : "");

    printf("Sending command: %s\n", buffer);

    send(socket_fd,
         buffer,
         strlen(buffer),
         0);
}


void receive_response(int client_fd)
{
    char buffer[MAX_CONTENT_LENGTH] = {0};

    int bytes_received = read(client_fd,
                              buffer,
                              sizeof(buffer) - 1);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';

        printf("Received response from server: %s\n",
               buffer);
    }
}