#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include "../../common/constants/constants.h"
#include "../../common/types/types.h"
#include "../../common/validation/validation.h"
#include "../socket/socket.h"

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

Operation build_command(Command *command)
{
    Operation operation = get_user_choice();

    command->operation = operation;

    if (operation == EXIT)
    {
        return operation;
    }

    get_file_path(command->path);

    if (operation_requires_content(operation))
    {
        get_file_content(command->content);
    }

    return operation;
}

void run_client(int client_fd)
{
    while (1)
    {
        Command command = {0};

        display_menu();

        Operation operation =
            build_command(&command);

        printf("You chose: %s\n",
               operation_names[operation]);

        send_command(client_fd,
                     &command);

        receive_response(client_fd);

        if (operation == EXIT)
        {
            printf("Exiting the program.\n");
            break;
        }
    }
}
