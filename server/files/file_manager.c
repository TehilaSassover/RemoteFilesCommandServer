#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include "../../common/constants/constants.h"
#include "../../common/types/types.h"
#include "../../common/validation/validation.h"



void initialize_working_directory(const char *path)
{
    if (mkdir(path, DIRECTORY_PERMISSIONS) == -1 && errno != EEXIST)
    {
        perror("Failed to create working directory");
        return;
    }
}



void create_directories(const char *working_directory, const char *path)
{
    char full_path[MAX_FULL_PATH_LENGTH];

    snprintf(full_path,
             sizeof(full_path),
             "%s/%s",
             working_directory,
             path);

    // Remove the file name from the path
    char *last_slash = strrchr(full_path, '/');

    if (last_slash == NULL)
    {
        return;
    }

    *last_slash = '\0';

    // Create directories one by one
    for (char *p = full_path + 1; *p; p++)
    {
        if (*p == '/')
        {
            *p = '\0';

            mkdir(full_path, DIRECTORY_PERMISSIONS);

            *p = '/';
        }
    }

    mkdir(full_path, DIRECTORY_PERMISSIONS);
}



ExecuteResult write_file(const Command *command,
                         const char *working_directory)
{
    char full_path[MAX_FULL_PATH_LENGTH];

    create_directories(working_directory, command->path);

    snprintf(full_path,
             sizeof(full_path),
             "%s/%s",
             working_directory,
             command->path);

    FILE *file = fopen(full_path, "w");

    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            return EXEC_DIRECTORY_NOT_FOUND;
        }

        return EXEC_OPEN_FAILED;
    }

    if (fprintf(file, "%s", command->content) < 0)
    {
        fclose(file);
        return EXEC_WRITE_FAILED;
    }

    fclose(file);

    return EXEC_SUCCESS;
}

ExecuteResult append_file(const Command *command,
                          const char *working_directory)
{
    char full_path[MAX_FULL_PATH_LENGTH];

    create_directories(working_directory,
                       command->path);

    snprintf(full_path,
             sizeof(full_path),
             "%s/%s",
             working_directory,
             command->path);

    FILE *file = fopen(full_path, "a");

    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            return EXEC_FILE_NOT_FOUND;
        }

        return EXEC_OPEN_FAILED;
    }

    if (fprintf(file, "%s", command->content) < 0)
    {
        fclose(file);
        return EXEC_WRITE_FAILED;
    }

    fclose(file);

    return EXEC_SUCCESS;
}

ExecuteResult create_file(const Command *command,
                          const char *working_directory)
{
    char full_path[MAX_FULL_PATH_LENGTH];

    create_directories(working_directory, command->path);

    snprintf(full_path,
             sizeof(full_path),
             "%s/%s",
             working_directory,
             command->path);

    if (access(full_path, F_OK) == 0)
    {
        return EXEC_FILE_ALREADY_EXISTS;
    }

    FILE *file = fopen(full_path, "w");

    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            return EXEC_DIRECTORY_NOT_FOUND;
        }

        return EXEC_CREATE_FAILED;
    }

    fclose(file);

    return EXEC_SUCCESS;
}

ExecuteResult delete_file(const Command *command,
                          const char *working_directory)
{
    char full_path[MAX_FULL_PATH_LENGTH];

    snprintf(full_path,
             sizeof(full_path),
             "%s/%s",
             working_directory,
             command->path);

    if (remove(full_path) != 0)
    {
        if (errno == ENOENT)
        {
            return EXEC_FILE_NOT_FOUND;
        }

        return EXEC_DELETE_FAILED;
    }

    return EXEC_SUCCESS;
}

ExecuteResult read_file(const Command *command,
                        const char *working_directory,
                        char *content)
{
    char full_path[MAX_FULL_PATH_LENGTH];

    snprintf(full_path,
             sizeof(full_path),
             "%s/%s",
             working_directory,
             command->path);

    FILE *file = fopen(full_path, "r");

    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            return EXEC_FILE_NOT_FOUND;
        }

        return EXEC_OPEN_FAILED;
    }

    size_t bytes_read = fread(content,
                              1,
                              MAX_CONTENT_LENGTH - 1,
                              file);

    if (ferror(file))
    {
        fclose(file);
        return EXEC_OPEN_FAILED;
    }

    content[bytes_read] = '\0';

    fclose(file);

    return EXEC_SUCCESS;
}

ExecuteResult execute_command(const Command *command,
                              const char *working_directory,
                              char *content)
{
    switch (command->operation)
    {
    case READ:
        return read_file(command, working_directory, content);
    case CREATE:
        return create_file(command, working_directory);

    case WRITE:
        return write_file(command, working_directory);

    case APPEND:
        return append_file(command, working_directory);

    case DELETE:
        return delete_file(command, working_directory);

    default:
        return EXEC_UNKNOWN_ERROR;
    }
}

