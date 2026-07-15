#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validation.h"
#include "../constants/constants.h"


bool is_valid_operation(const char *input)
{
    int value;
    char extra;

    if (input == NULL)
    {
        printf("Operation cannot be NULL.\n");
        return false;
    }

    if (sscanf(input, "%d %c", &value, &extra) != 1)
    {
        printf("Operation must be a number.\n");
        return false;
    }

    if (value < 0 || value >= OPERATION_COUNT)
    {
        printf("Operation must be between 0 and %d.\n",
               OPERATION_COUNT - 1);
        return false;
    }

    return true;
}

bool is_valid_path(const char *path)
{
    if (path == NULL)
    {
        printf("Path cannot be NULL. ");
        return false;
    }
    if (strlen(path) == 0)
    {
        printf("Path cannot be empty.\n");
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
            printf("Path contains forbidden character: '%c'.\n", forbidden_chars[i]);
            return false;
        }
    }
    return true;
}

bool is_valid_content(const char *content)
{
    if (content == NULL)
    {
        printf("Content cannot be NULL.\n");
        return false;
    }

    if (strlen(content) == 0)
    {
        printf("Content cannot be empty.\n");
        return false;
    }

    if (strlen(content) >= MAX_CONTENT_LENGTH)
    {
        printf("Content is too long.\n");
        return false;
    }

    return true;
}
