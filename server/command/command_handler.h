#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <stdbool.h>
#include "../../common/types/types.h"
#include "../../common/validation/validation.h"


/**
 * @brief Parses a serialized command received from the client.
 *
 * @param command_str Received command string.
 * @param command Command structure to fill.
 *
 * @return true if parsing succeeded, false otherwise.
 */
bool parse_command(const char *command_str,
                   Command *command);


/**
 * @brief Validates a client command.
 *
 * @param command Command to validate.
 *
 * @return Validation result.
 */
ValidationResult validate_command(const Command *command);


/**
 * @brief Handles commands received from a client.
 *
 * Receives, validates and executes client requests.
 *
 * @param client_fd Client socket descriptor.
 * @param working_directory Server storage directory.
 */
void handle_client_commands(int client_fd,
                            const char *working_directory);


/**
 * @brief Sends execution result back to client.
 */
void send_execute_response(int client_fd,
                           ExecuteResult result,
                           Operation operation,
                           const char *file_content);


#endif