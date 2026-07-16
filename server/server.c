#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include "../common/constants/constants.h"
#include "../common/types/types.h"
#include "../common/validation/validation.h"
#include "../common/validation/validation.c"

void initialize_working_directory(const char *path)
{
    if (mkdir(path, 0777) == -1 && errno != EEXIST)
    {
        perror("Failed to create working directory");
        return;
    }
}

bool parse_command(const char *command_str, Command *command)
{
    // Find the three separators '|'
    const char *first = strchr(command_str, SEPARATOR_CHAR);
    const char *second = first ? strchr(first + 1, SEPARATOR_CHAR) : NULL;
    const char *third = second ? strchr(second + 1, SEPARATOR_CHAR) : NULL;

    // Validate command format
    // Expected format:
    // operation|path_length|content_length|path+content
    if (!first || !second || !third)
    {
        printf("Invalid command format.\n");
        return false;
    }

    /*
        Extract operation

        Example:
        2|9|11|notes.txtHello World
        ^
        |
        first
    */
    int operation_length = first - command_str;

    char operation_str[10];

    memcpy(operation_str,
           command_str,
           operation_length);

    operation_str[operation_length] = '\0';

    command->operation = (Operation)atoi(operation_str);

    /*
        Extract path length

        Value is between first and second '|'
    */
    int path_length_size = second - first - 1;

    char path_length_str[10];

    memcpy(path_length_str,
           first + 1,
           path_length_size);

    path_length_str[path_length_size] = '\0';

    int path_length = atoi(path_length_str);

    /*
        Extract content length

        Value is between second and third '|'
    */
    int content_length_size = third - second - 1;

    char content_length_str[10];

    memcpy(content_length_str,
           second + 1,
           content_length_size);

    content_length_str[content_length_size] = '\0';

    int content_length = atoi(content_length_str);

    // Validate lengths before copying data
    if (path_length >= MAX_PATH_LENGTH)
    {
        printf("Path is too long.\n");
        return false;
    }

    if (content_length >= MAX_CONTENT_LENGTH)
    {
        printf("Content is too long.\n");
        return false;
    }

    /*
        Extract path and content

        After third '|':

        notes.txtHello World
        ^^^^^^^^^
        path_length = 9
    */

    memcpy(command->path,
           third + 1,
           path_length);

    command->path[path_length] = '\0';

    /*
        Content starts immediately after the path
    */

    memcpy(command->content,
           third + 1 + path_length,
           content_length);

    command->content[content_length] = '\0';

    return true;
}

ValidationResult validate_command(const Command *command)
{
    if (command == NULL)
    {
        return INVALID_NULL;
    }

    ValidationResult result;

    result = validate_operation(command->operation);

    if (result != VALID)
    {
        return result;
    }

    result = validate_path(command->path);

    if (result != VALID)
    {
        return result;
    }

    if (operation_requires_content(command->operation))
    {
        result = validate_content(command->content);

        if (result != VALID)
        {
            return result;
        }
    }

    return VALID;
}

int main()
{

    char *working_directory = "../storage";
    initialize_working_directory(working_directory);

    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
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
    if (is_bound == -1)
    {
        perror("Failed to bind socket");
        return 1;
    }
    printf("Server bound to address and port\n");

    // Listen for incoming connections
    int backlog = 5;
    int is_listening = listen(server_fd, backlog);
    if (is_listening == -1)
    {
        perror("Failed to listen for connections");
        return 1;
    }
    printf("Server listening for connections...\n");

    // Accept a connection
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        perror("Failed to accept connection");
        return 1;
    }
    printf("Accepted a connection from client with file descriptor: %d\n", client_fd);

    char buffer[1024] = {0};
    read(client_fd, buffer, sizeof(buffer));
    printf("Received message from client: %s\n", buffer);

    Command command;

    if (parse_command(buffer, &command))
    {
        ValidationResult result = validate_command(&command);

        if (result != VALID)
        {
            printf("Invalid command received\n");

            const char *response = "Invalid command";
            send(client_fd, response, strlen(response), 0);

            return 1;
        }

        printf("Operation: %d\n", command.operation);
        printf("Path: %s\n", command.path);
        printf("Content: %s\n", command.content);

        // כאן עוברים לביצוע הפעולה
    }

    // Send a response back to the client
    const char *response = "Hello from server!";
    send(client_fd, response, strlen(response), 0);

    // Close the sockets
    close(client_fd);
    close(server_fd);

    return 0;
}