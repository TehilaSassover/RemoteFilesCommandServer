#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>

typedef enum
{
    EXIT = 0,
    CREATE,
    WRITE,
    APPEND,
    READ,
    DELETE
} Operation;

const char *operation_names[] =
    {
        "EXIT",
        "CREATE",
        "WRITE",
        "APPEND",
        "READ",
        "DELETE"};

const int OPERATION_COUNT = sizeof(operation_names) / sizeof(operation_names[0]);
#define SPACE ' '
#define HOME_DIRECTORY '~'
#define ROOT_DIRECTORY '/'
#define PARENT_DIRECTORY ".."

const char *operation_descriptions[] =
    {
        "Exit the program",
        "Create a new file",
        "Write to a file",
        "Append to a file",
        "Read a file",
        "Delete a file"};

const char forbidden_chars[] = {
    '?',
    '*',
    '|',
    '<',
    '>',
    ':',
    '"',
    '\\'};

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

int get_user_choice()
{
    int choice;
    while (1)
    {
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        while (getchar() != '\n')
            ;

        if (choice >= 0 && choice < OPERATION_COUNT)
        {
            return choice;
        }

        printf("Please enter a number between 0 and %d.\n", OPERATION_COUNT - 1);
    }
}

#define MAX_PATH_LENGTH 256

bool is_valid_path(const char *path)
{
    if (path == NULL)
    {
        printf("Path cannot be NULL. ");
        return false;
    }
    if (strlen(path) >= MAX_PATH_LENGTH)
    {
        printf("Path is too long. ");
        return false;
    }
    if (strchr(path, SPACE) != NULL)
    {
        printf("Path cannot contain spaces. ");
        return false;
    }
    if (path[0] == ROOT_DIRECTORY)
    {
        printf("Absolute paths are not allowed.\n");
        return false;
    }
    if (strchr(path, HOME_DIRECTORY) != NULL)
    {
        printf("Path cannot contain '~'.\n");
        return false;
    }
    if (strstr(path, PARENT_DIRECTORY) != NULL)
    {
        printf("Path cannot access parent directories.\n");
        return false;
    }
    for (int i = 0; forbidden_chars[i] != '\0'; i++)
    {
        if (strchr(path, forbidden_chars[i]) != NULL)
        {
            printf("Path contains forbidden character: %c. ", forbidden_chars[i]);
            return false;
        }
    }
    return true;
}

void get_file_path(char *path)
{
    while (1)
    {
        printf("Enter the file path: ");

        scanf("%255s", path);
        while (getchar() != '\n')
            ;
        if (is_valid_path(path))
        {
            return;
        }

        printf("Please try again.\n");
    }
}

int main()
{
    // Create a socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
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
    if (result == -1)
    {
        perror("Failed to connect to server");
        return 1;
    }
    printf("Client connected to server\n");

    display_menu();
    int choice = get_user_choice();
    printf("You chose: %s\n", operation_names[choice]);

    if (choice == EXIT)
    {
        printf("Exiting the program.\n");
        return 0;
    }

    char path[MAX_PATH_LENGTH];

    get_file_path(path);

    printf("path: %s\n", path);

    Operation operation = get_user_choice();

   

    // Send a message to the server
    const char *message = "Hello from client!";
    send(client_fd, message, strlen(message), 0);

    // Receive a response from the server
    char buffer[1024] = {0};
    read(client_fd, buffer, sizeof(buffer));
    printf("Received response from server: %s\n", buffer);
    return 0;
}