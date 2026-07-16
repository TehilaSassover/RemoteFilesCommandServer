#include <string.h>

#include "validation.h"
#include "../constants/constants.h"



ValidationResult validate_operation(Operation operation)
{
    if (operation < EXIT || operation >= OPERATION_COUNT)
    {
        return INVALID_OPERATION;
    }

    return VALID;
}



ValidationResult validate_path(const char *path)
{
    if (path == NULL)
    {
        return INVALID_NULL;
    }


    if (strlen(path) == 0)
    {
        return INVALID_EMPTY;
    }


    if (strlen(path) >= MAX_PATH_LENGTH)
    {
        return INVALID_TOO_LONG;
    }


    if (strchr(path, SPACE) != NULL)
    {
        return INVALID_SPACE;
    }


    if (path[0] == ROOT_DIRECTORY)
    {
        return INVALID_ABSOLUTE_PATH;
    }


    if (strchr(path, HOME_DIRECTORY) != NULL)
    {
        return INVALID_HOME_PATH;
    }


    if (strstr(path, PARENT_DIRECTORY) != NULL)
    {
        return INVALID_PARENT_PATH;
    }


    for (int i = 0; forbidden_chars[i] != '\0'; i++)
    {
        if (strchr(path, forbidden_chars[i]) != NULL)
        {
            return INVALID_FORBIDDEN_CHAR;
        }
    }


    return VALID;
}



ValidationResult validate_content(const char *content)
{
    if (content == NULL)
    {
        return INVALID_NULL;
    }


    if (strlen(content) == 0)
    {
        return INVALID_EMPTY;
    }


    if (strlen(content) >= MAX_CONTENT_LENGTH)
    {
        return INVALID_TOO_LONG;
    }


    return VALID;
}

bool operation_requires_content(Operation operation)
{
    return operation == WRITE || operation == APPEND;
}