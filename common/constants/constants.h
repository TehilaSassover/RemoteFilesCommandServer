#ifndef CONSTANTS_H
#define CONSTANTS_H


/*********************************
 * Server configuration constants
 *********************************/


/**
 * @brief Port used by the server socket.
 */
#define SERVER_PORT 8080


/**
 * @brief Ip used by the server socket.
 */
#define SERVER_IP "127.0.0.1"

/**
 * @brief Maximum number of pending client connections.
 *
 * Used as the backlog parameter in listen().
 */
#define SERVER_BACKLOG 5


/**
 * @brief Permissions used when creating directories.
 *
 * Represents read, write and execute permissions
 * for owner, group and others.
 */
#define DIRECTORY_PERMISSIONS 0777



/*********************************
 * Buffer size constants
 *********************************/


/**
 * @brief Maximum length of user input.
 */
#define MAX_INPUT_LENGTH 100


/**
 * @brief Maximum size used for temporary numeric strings.
 *
 * Used when parsing numeric values from commands.
 */
#define NUMBER_BUFFER_SIZE 10


/**
 * @brief Maximum length allowed for a file path.
 */
#define MAX_PATH_LENGTH 256


/**
 * @brief Maximum length allowed for file content.
 */
#define MAX_CONTENT_LENGTH 1024


/**
 * @brief Maximum size of a communication buffer.
 *
 * Used for sending and receiving data through sockets.
 */
#define MAX_BUFFER_SIZE 2048


/**
 * @brief Maximum length allowed for a complete file path.
 *
 * Includes the working directory and the relative file path.
 */
#define MAX_FULL_PATH_LENGTH (MAX_PATH_LENGTH * 2)



/*********************************
 * File system validation constants
 *********************************/


/**
 * @brief Space character.
 *
 * Used to validate file paths.
 */
#define SPACE ' '


/**
 * @brief Home directory symbol.
 *
 * Paths containing '~' are not allowed.
 */
#define HOME_DIRECTORY '~'


/**
 * @brief Root directory symbol.
 *
 * Absolute paths starting with '/' are not allowed.
 */
#define ROOT_DIRECTORY '/'


/**
 * @brief Parent directory pattern.
 *
 * Prevents access to directories outside the working directory.
 */
#define PARENT_DIRECTORY ".."



/*********************************
 * Communication protocol constants
 *********************************/


/**
 * @brief Command separator character.
 *
 * Separates fields in messages exchanged between client and server.
 *
 * Example:
 * operation|path_length|content_length|path+content
 */
#define SEPARATOR_CHAR '|'


/**
 * @brief Command separator string.
 *
 * String representation of the command separator.
 * Used when building protocol messages with snprintf().
 */
#define SEPARATOR_STRING "|"



/*********************************
 * Constant arrays
 *********************************/


/**
 * @brief Array containing operation names.
 *
 * Used for displaying available operations to the user.
 */
extern const char *operation_names[];


/**
 * @brief Array containing operation descriptions.
 *
 * Used for displaying explanations of available operations.
 */
extern const char *operation_descriptions[];


/**
 * @brief Array containing forbidden characters for paths.
 *
 * Used during path validation.
 */
extern const char forbidden_chars[];


#endif