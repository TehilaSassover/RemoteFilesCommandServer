#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>
#include "../types/types.h"


typedef enum
{
    VALID,

    INVALID_NULL,
    INVALID_EMPTY,
    INVALID_TOO_LONG,

    INVALID_OPERATION,

    INVALID_SPACE,
    INVALID_ABSOLUTE_PATH,
    INVALID_HOME_PATH,
    INVALID_PARENT_PATH,
    INVALID_FORBIDDEN_CHAR

} ValidationResult;


ValidationResult validate_operation(Operation operation);

ValidationResult validate_path(const char *path);

ValidationResult validate_content(const char *content);

bool operation_requires_content(Operation operation);


#endif