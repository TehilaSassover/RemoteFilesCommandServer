#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    // Create a socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Failed to create socket");
        return 1;
    }
    printf("Client socket file descriptor: %d\n", client_fd);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary form
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // Connect to the server
    int result = connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (result == -1) {
        perror("Failed to connect to server");
        return 1;
    }
    printf("Client connected to server\n");

    // Send a message to the server
    const char *message = "Hello from client!";
    send(client_fd, message, strlen(message), 0);

    // Receive a response from the server
    char buffer[1024] = {0};
    read(client_fd, buffer, sizeof(buffer));
    printf("Received response from server: %s\n", buffer);
    return 0;
}