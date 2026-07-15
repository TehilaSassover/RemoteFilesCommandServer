#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

void initialize_working_directory(const char *path) {
    if (mkdir(path, 0777) == -1 && errno != EEXIST) {
        perror("Failed to create working directory");
        return;
    }
}

int main() {
    
    char *working_directory = "../storage";
    initialize_working_directory(working_directory);
    

    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Failed to create socket");
        return 1;
    }
    printf("Server socket file descriptor: %d\n", server_fd);

    // Bind the socket to an address and port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    int is_bound = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (is_bound == -1) {
        perror("Failed to bind socket");
        return 1;
    }
    printf("Server bound to address and port\n");

    // Listen for incoming connections
    int backlog = 5;
    int is_listening = listen(server_fd, backlog);
    if (is_listening == -1) {
        perror("Failed to listen for connections");
        return 1;
    }
    printf("Server listening for connections...\n");

    // Accept a connection
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("Failed to accept connection");
        return 1;
    }
    printf("Accepted a connection from client with file descriptor: %d\n", client_fd);
    
    char buffer[1024] = {0};
    read(client_fd, buffer, sizeof(buffer));
    printf("Received message from client: %s\n", buffer);
    
    // Send a response back to the client
    const char *response = "Hello from server!";
    send(client_fd, response, strlen(response), 0);

    // Close the sockets
    close(client_fd);
    close(server_fd);


    return 0;
}