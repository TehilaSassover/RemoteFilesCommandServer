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
#include "../../common/types/types.h"
#include "../../common/validation/validation.h"
#include "../files/file_manager.h"
#include "../socket/socket.h"

bool parse_command(const char *command_str, Command *command)
{
    const char *first = strchr(command_str, SEPARATOR_CHAR);
    const char *second = first ? strchr(first + 1, SEPARATOR_CHAR) : NULL;
    const char *third = second ? strchr(second + 1, SEPARATOR_CHAR) : NULL;

    if (!first || !second || !third)
    {
        printf("Invalid command format.\n");
        return false;
    }

    size_t operation_length = first - command_str;

    char operation_str[NUMBER_BUFFER_SIZE];

    if (operation_length >= sizeof(operation_str))
        return false;

    memcpy(operation_str,
           command_str,
           operation_length);

    operation_str[operation_length] = '\0';

    command->operation = (Operation)atoi(operation_str);

    size_t path_length_size = second - first - 1;

    char path_length_str[NUMBER_BUFFER_SIZE];

    if (path_length_size >= sizeof(path_length_str))
        return false;

    memcpy(path_length_str,
           first + 1,
           path_length_size);

    path_length_str[path_length_size] = '\0';

    int path_length = atoi(path_length_str);

    size_t content_length_size = third - second - 1;

    char content_length_str[NUMBER_BUFFER_SIZE];

    if (content_length_size >= sizeof(content_length_str))
        return false;

    memcpy(content_length_str,
           second + 1,
           content_length_size);

    content_length_str[content_length_size] = '\0';

    int content_length = atoi(content_length_str);

    if (path_length < 0 || content_length < 0)
        return false;

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

    size_t data_length = strlen(third + 1);

    if ((size_t)(path_length + content_length) > data_length)
    {
        return false;
    }
    memcpy(command->path,
           third + 1,
           path_length);

    command->path[path_length] = '\0';

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

const char *get_validation_message(ValidationResult result)
{
    switch (result)
    {
    case INVALID_NULL:
        return "ERROR|Value cannot be NULL";

    case INVALID_EMPTY:
        return "ERROR|Value cannot be empty";

    case INVALID_TOO_LONG:
        return "ERROR|Value is too long";

    case INVALID_OPERATION:
        return "ERROR|Invalid operation";

    case INVALID_SPACE:
        return "ERROR|Path cannot contain spaces";

    case INVALID_ABSOLUTE_PATH:
        return "ERROR|Absolute paths are not allowed";

    case INVALID_HOME_PATH:
        return "ERROR|Path cannot contain ~";

    case INVALID_PARENT_PATH:
        return "ERROR|Parent directory access is not allowed";

    case INVALID_FORBIDDEN_CHAR:
        return "ERROR|Path contains forbidden character";

    default:
        return "ERROR|Unknown validation error";
    }
}

const char *get_execute_message(ExecuteResult result)
{
    switch (result)
    {
    case EXEC_SUCCESS:
        return "SUCCESS|Operation completed successfully";

    case EXEC_FILE_NOT_FOUND:
        return "ERROR|File was not found";

    case EXEC_DIRECTORY_NOT_FOUND:
        return "ERROR|Directory was not found";

    case EXEC_CREATE_FAILED:
        return "ERROR|Failed to create file";

    case EXEC_OPEN_FAILED:
        return "ERROR|Failed to open file";

    case EXEC_WRITE_FAILED:
        return "ERROR|Failed to write content";

    case EXEC_DELETE_FAILED:
        return "ERROR|Failed to delete file";

    case EXEC_FILE_ALREADY_EXISTS:
        return "ERROR|File already exists";

    default:
        return "ERROR|Unknown server error";
    }
}


void send_execute_response(int client_fd, ExecuteResult result, Operation operation, const char *file_content)
{
    if (result != EXEC_SUCCESS)
    {
        const char *response = get_execute_message(result);

        send(client_fd,
             response,
             strlen(response),
             0);

        return;
    }

    if (operation == READ)
    {
        char response[MAX_BUFFER_SIZE];

        snprintf(response,
                 sizeof(response),
                 "SUCCESS|%s",
                 file_content);

        send(client_fd,
             response,
             strlen(response),
             0);

        return;
    }

    const char *response = get_execute_message(result);

    send(client_fd,
         response,
         strlen(response),
         0);
}

void handle_client_commands(int client_fd, const char *working_directory)
{
    while (1)
    {
        char buffer[MAX_BUFFER_SIZE] = {0};

        int bytes_received = read(client_fd,
                                  buffer,
                                  sizeof(buffer) - 1);

        if (bytes_received <= 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';

        printf("Received: %s\n", buffer);

        Command command;

        if (!parse_command(buffer, &command))
        {
            const char *response = "Invalid command format";

            send(client_fd,
                 response,
                 strlen(response),
                 0);

            continue;
        }

        if (command.operation == EXIT)
        {
            const char *response = "Goodbye";

            send(client_fd,
                 response,
                 strlen(response),
                 0);

            printf("Client requested exit.\n");

            break;
        }

        ValidationResult validation_result =
            validate_command(&command);

        if (validation_result != VALID)
        {
            const char *response =
                get_validation_message(validation_result);

            send(client_fd,
                 response,
                 strlen(response),
                 0);

            continue;
        }

        char file_content[MAX_CONTENT_LENGTH] = {0};

        ExecuteResult execute_result =
            execute_command(&command,
                            working_directory,
                            file_content);

        send_execute_response(client_fd,
                              execute_result,
                              command.operation,
                              file_content);
    }
}
