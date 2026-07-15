#ifndef TYPES_H
#define TYPES_H

#include "../constants/constants.h"

typedef enum
{
    EXIT = 0,
    CREATE,
    WRITE,
    APPEND,
    READ,
    DELETE,
    OPERATION_COUNT
} Operation;

typedef struct
{
    Operation operation;
    char path[MAX_PATH_LENGTH];
    char content[MAX_CONTENT_LENGTH];
} Command;

#endif