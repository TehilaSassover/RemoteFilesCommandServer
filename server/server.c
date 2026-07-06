#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Server socket file descriptor: %d\n", server_fd);

    // Bind the socket to an address and port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    printf("Server bound");

    // Listen for incoming connections
    int backlog = 5;
    listen(server_fd, backlog);
    printf("Server listening for connections...\n");

    // Accept a connection
    int client_fd = accept(server_fd, NULL, NULL);
    printf("Accepted a connection from client with file descriptor: %d\n", client_fd);
    
    char buffer[1024] = {0};
    read(client_fd, buffer, sizeof(buffer));
    printf("Received message from client: %s\n", buffer);

    // Close the sockets
    close(client_fd);
    close(server_fd);


    return 0;
}