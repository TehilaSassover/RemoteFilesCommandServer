#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

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
    "DELETE"
};

const int OPERATION_COUNT = sizeof(operation_names) / sizeof(operation_names[0]);

const char *operation_descriptions[] =
{
    "Exit the program",
    "Create a new file",
    "Write to a file",
    "Append to a file",
    "Read a file",
    "Delete a file"
};

void print_menu(){

}

void display_menu(){
    
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


int get_user_choice(){
    int choice;
    while(1){
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice >= 0 && choice < OPERATION_COUNT)
        {
            return choice;
        }

        printf("Please enter a number between 0 and %d.\n", OPERATION_COUNT - 1);
    }
}



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

    display_menu();
    int choice = get_user_choice();
    printf("You chose: %s\n", operation_names[choice]);
    
    if(choice == EXIT){
        printf("Exiting the program.\n");
        return 0;
    }




    // Send a message to the server
    const char *message = "Hello from client!";
    send(client_fd, message, strlen(message), 0);

    // Receive a response from the server
    char buffer[1024] = {0};
    read(client_fd, buffer, sizeof(buffer));
    printf("Received response from server: %s\n", buffer);
    return 0;
}