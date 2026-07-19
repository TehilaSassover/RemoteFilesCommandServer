#ifndef SOCKET_H
#define SOCKET_H


#include "../../common/types/types.h"

/**
 * @brief Creates a TCP connection to the server.
 *
 * @param ip The server IPv4 address.
 * @param port The server port.
 * @return The connected socket file descriptor on success, or -1 on failure.
 */
int connect_to_server(const char *ip, int port);

/**
 * @brief Serializes a command and sends it to the server.
 *
 * The command is serialized using the following protocol:
 * operation|path_length|content_length|pathcontent
 *
 * @param socket_fd The connected socket descriptor.
 * @param command Pointer to the command structure to serialize and send.
 */
void send_command(int socket_fd, Command *command);


/**
 * @brief Receives server response and prints it.
 *
 * @param client_fd Client socket descriptor.
 */
void receive_response(int client_fd);



#endif
