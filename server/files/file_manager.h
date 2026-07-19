#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../../common/types/types.h"
#include "../../common/validation/validation.h"


/**
 * @brief Creates the server working directory if it does not exist.
 *
 * @param path Path of the directory to initialize.
 */
void initialize_working_directory(const char *path);

/**
 * @brief Creates all required directories for a file path.
 *
 * Builds the full path using the working directory and the given file path,
 * then creates missing directories recursively.
 *
 * @param working_directory Base directory where files are stored.
 * @param path Relative file path inside the working directory.
 */
void create_directories(const char *working_directory,
                        const char *path);


/**
 * @brief Writes content to a file.
 *
 * Creates the required directories if they do not exist,
 * then writes the given content to the file.
 *
 * @param command Command containing file path and content.
 * @param working_directory Base directory where files are stored.
 *
 * @return ExecuteResult indicating whether the operation succeeded or failed.
 */
ExecuteResult write_file(const Command *command,
                         const char *working_directory);


/**
 * @brief Appends content to an existing file.
 *
 * Opens the file in append mode and adds the provided content
 * at the end of the file.
 *
 * @param command Command containing file path and content.
 * @param working_directory Base directory where files are stored.
 *
 * @return ExecuteResult indicating whether the operation succeeded or failed.
 */
ExecuteResult append_file(const Command *command,
                          const char *working_directory);


/**
 * @brief Creates a new empty file.
 *
 * Creates the required directories and creates a new file.
 * Returns an error if the file already exists.
 *
 * @param command Command containing the file path.
 * @param working_directory Base directory where files are stored.
 *
 * @return ExecuteResult indicating whether the operation succeeded or failed.
 */
ExecuteResult create_file(const Command *command,
                          const char *working_directory);


/**
 * @brief Deletes a file.
 *
 * Removes the requested file from the working directory.
 *
 * @param command Command containing the file path.
 * @param working_directory Base directory where files are stored.
 *
 * @return ExecuteResult indicating whether the operation succeeded or failed.
 */
ExecuteResult delete_file(const Command *command,
                          const char *working_directory);


/**
 * @brief Reads file content.
 *
 * Opens the requested file and copies its content into the provided buffer.
 *
 * @param command Command containing the file path.
 * @param working_directory Base directory where files are stored.
 * @param content Buffer where the file content will be stored.
 *
 * @return ExecuteResult indicating whether the operation succeeded or failed.
 */
ExecuteResult read_file(const Command *command,
                        const char *working_directory,
                        char *content);


/**
 * @brief Executes a file operation based on the command type.
 *
 * Determines which file operation should be performed
 * according to the operation field in the Command structure.
 *
 * Supported operations:
 * - CREATE
 * - WRITE
 * - APPEND
 * - READ
 * - DELETE
 *
 * @param command Command containing the requested operation.
 * @param working_directory Base directory where files are stored.
 * @param content Buffer used for storing read file content.
 *
 * @return ExecuteResult indicating whether the operation succeeded or failed.
 */
ExecuteResult execute_command(const Command *command,
                              const char *working_directory,
                              char *content);


#endif