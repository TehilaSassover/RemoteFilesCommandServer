#ifndef INPUT_H
#define INPUT_H

/**
 * @brief Displays the list of available operations.
 */
void display_menu();

/**
 * @brief Prints a validation error message.
 *
 * @param result The validation result to display.
 */
void print_validation_error(ValidationResult result);

/**
 * @brief Reads and validates the user's operation choice.
 *
 * Keeps asking until the user enters a valid operation.
 *
 * @return The selected operation.
 */
Operation get_user_choice();

/**
 * @brief Reads and validates a file path from the user.
 *
 * Keeps asking until a valid path is entered.
 *
 * @param path Buffer that receives the validated file path.
 */
void get_file_path(char *path);

/**
 * @brief Reads and validates file content from the user.
 *
 * Keeps asking until valid content is entered.
 *
 * @param content Buffer that receives the validated content.
 */
void get_file_content(char *content);


/**
 * @brief Creates a command according to user input.
 *
 * @param command Command structure to fill.
 *
 * @return Selected operation.
 */
Operation build_command(Command *command);

/**
 * @brief Runs the client interaction loop.
 *
 * @param client_fd Connected socket descriptor.
 */
void run_client(int client_fd);

#endif