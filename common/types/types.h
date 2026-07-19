#ifndef TYPES_H
#define TYPES_H

#include "../constants/constants.h"


/**
 * @brief Represents the available file operations.
 *
 * Defines all commands that can be sent from the client
 * to the server.
 */
typedef enum
{
    EXIT = 0,   /**< Close the client-server connection. */
    CREATE,     /**< Create a new file. */
    WRITE,      /**< Write content to a file. */
    APPEND,     /**< Append content to an existing file. */
    READ,       /**< Read file content. */
    DELETE,     /**< Delete a file. */

    OPERATION_COUNT /**< Number of available operations. */
} Operation;


/**
 * @brief Represents a command sent between client and server.
 *
 * Contains the requested operation and the relevant file data.
 * The path field specifies the target file path, and the content
 * field contains data required for operations such as WRITE and APPEND.
 */
typedef struct
{
    Operation operation;              /**< The requested file operation. */
    char path[MAX_PATH_LENGTH];       /**< The file path related to the operation. */
    char content[MAX_CONTENT_LENGTH]; /**< The file content if required. */
} Command;


#endif