#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>
#include "../types/types.h"


/**
 * @brief Represents the result of a validation check.
 *
 * Used by validation functions to indicate whether the input is valid
 * or to specify the reason for validation failure.
 */
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


/**
 * @brief Represents the result of executing a file operation.
 *
 * Used by server execution functions to report whether an operation
 * succeeded or failed and why.
 */
typedef enum
{
    EXEC_SUCCESS,

    EXEC_FILE_NOT_FOUND,
    EXEC_DIRECTORY_NOT_FOUND,

    EXEC_FILE_ALREADY_EXISTS,

    EXEC_CREATE_FAILED,
    EXEC_OPEN_FAILED,
    EXEC_WRITE_FAILED,
    EXEC_DELETE_FAILED,

    EXEC_UNKNOWN_ERROR

} ExecuteResult;


/**
 * @brief Validates an operation type.
 *
 * Checks whether the requested operation exists and is supported.
 *
 * @param operation The operation to validate.
 *
 * @return VALID if the operation is valid,
 *         otherwise the relevant validation error.
 */
ValidationResult validate_operation(Operation operation);


/**
 * @brief Validates a file path.
 *
 * Checks that the path is not empty, does not exceed the maximum length,
 * and does not contain forbidden characters or invalid path patterns.
 *
 * @param path The path to validate.
 *
 * @return VALID if the path is valid,
 *         otherwise the relevant validation error.
 */
ValidationResult validate_path(const char *path);


/**
 * @brief Validates file content.
 *
 * Checks that the content is valid according to the project rules.
 *
 * @param content The content to validate.
 *
 * @return VALID if the content is valid,
 *         otherwise the relevant validation error.
 */
ValidationResult validate_content(const char *content);


/**
 * @brief Checks whether an operation requires content.
 *
 * Operations such as WRITE and APPEND require content,
 * while operations such as CREATE, READ, DELETE and EXIT do not.
 *
 * @param operation The operation to check.
 *
 * @return true if the operation requires content,
 *         false otherwise.
 */
bool operation_requires_content(Operation operation);


#endif