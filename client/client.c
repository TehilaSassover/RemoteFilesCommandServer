#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include "../common/constants/constants.h"
#include "../common/types/types.h"
#include "../common/validation/validation.h"

int connect_to_server(const char *ip, int port)
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1)
    {
        perror("Failed to create socket");
        return -1;
    }

    printf("Client socket file descriptor: %d\n", client_fd);

    struct sockaddr_in server_address;

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

void display_menu()
{

    printf("Choose an operation:\n");

    for (int i = 0; i < OPERATION_COUNT; i++)
    {
        printf("%d. %s - %s\n",
               i,
               operation_names[i],
               operation_descriptions[i]);
    }

    printf("Your choice: ");
}

void print_validation_error(ValidationResult result)
{
    switch (result)
    {
    case INVALID_NULL:
        printf("Input cannot be NULL.\n");
        break;

    case INVALID_EMPTY:
        printf("Input cannot be empty.\n");
        break;

    case INVALID_TOO_LONG:
        printf("Input is too long.\n");
        break;

    case INVALID_OPERATION:
        printf("Invalid operation.\n");
        break;

    case INVALID_SPACE:
        printf("Path cannot contain spaces.\n");
        break;

    case INVALID_ABSOLUTE_PATH:
        printf("Absolute paths are not allowed.\n");
        break;

    case INVALID_HOME_PATH:
        printf("Path cannot contain '~'.\n");
        break;

    case INVALID_PARENT_PATH:
        printf("Path cannot access parent directories.\n");
        break;

    case INVALID_FORBIDDEN_CHAR:
        printf("Path contains forbidden characters.\n");
        break;

    default:
        printf("Invalid input.\n");
    }
}

Operation get_user_choice()
{
    char input[MAX_INPUT_LENGTH];
    int operation;

    while (1)
    {
        fgets(input, MAX_INPUT_LENGTH, stdin);
        input[strcspn(input, "\n")] = '\0';

        if (sscanf(input, "%d", &operation) == 1)
        {
            ValidationResult result =
                validate_operation((Operation)operation);

            if (result == VALID)
            {
                return (Operation)operation;
            }

            print_validation_error(result);
        }
        else
        {
            printf("Operation must be a number.\n");
        }

        printf("Please try again.\n");
    }
}

void get_file_path(char *path)
{
    while (1)
    {
        printf("Enter the file path: ");

        fgets(path, MAX_PATH_LENGTH, stdin);
        path[strcspn(path, "\n")] = '\0';

        ValidationResult result = validate_path(path);

        if (result == VALID)
        {
            return;
        }

        print_validation_error(result);

        printf("Please try again.\n");
    }
}


void get_file_content(char *content)
{
    while (1)
    {
        printf("Enter the content: ");

        fgets(content, MAX_CONTENT_LENGTH, stdin);
        content[strcspn(content, "\n")] = '\0';

        ValidationResult result = validate_content(content);

        if (result == VALID)
        {
            return;
        }

        print_validation_error(result);

        printf("Please try again.\n");
    }
}

void send_command(int socket_fd, Command *command)
{
    char buffer[MAX_BUFFER_SIZE];

    int path_length = strlen(command->path);
    int content_length = (command->content != NULL) ? strlen(command->content) : 0;

    snprintf(buffer,
             MAX_BUFFER_SIZE,
             "%d|%d|%d|%s%s",
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

int main()
{
    int client_fd = connect_to_server("127.0.0.1", 8080);

    if (client_fd == -1)
    {
        return 1;
    }

    display_menu();
    Operation operation = get_user_choice();
    printf("You chose: %s\n", operation_names[operation]);
    if (operation == EXIT)
    {
        printf("Exiting the program.\n");
        return 0;
    }

    char path[MAX_PATH_LENGTH];

    get_file_path(path);

    printf("path: %s\n", path);

    char content[MAX_CONTENT_LENGTH] = {0};

    if (operation_requires_content(operation))
    {
        get_file_content(content);
    }

    Command command;
    command.operation = operation;
    strcpy(command.path, path);
    strcpy(command.content, content);

    // Send the command to the server
    send_command(client_fd, &command);

    // Receive a response from the server
    char buffer[1024] = {0};
    read(client_fd, buffer, sizeof(buffer));
    printf("Received response from server: %s\n", buffer);
    return 0;
}