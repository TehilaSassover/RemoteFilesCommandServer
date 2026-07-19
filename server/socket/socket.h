#ifndef SOCKET_H
#define SOCKET_H


/**
 * @brief Creates and configures a TCP server socket.
 *
 * Creates a socket using IPv4 and TCP protocol,
 * binds it to the given port, and starts listening
 * for incoming client connections.
 *
 * The function performs the following steps:
 * - Creates a socket using socket().
 * - Binds the socket to all available network interfaces.
 * - Starts listening for incoming connections.
 *
 * @param port Port number on which the server will listen.
 *
 * @return Server socket file descriptor on success,
 *         -1 on failure.
 */
int create_server_socket(int port);


/**
 * @brief Accepts an incoming client connection.
 *
 * Waits for a client to connect to the server socket
 * and creates a new socket descriptor for communication
 * with that client.
 *
 * @param server_fd Server socket file descriptor returned
 *                  by create_server_socket().
 *
 * @return Client socket file descriptor on success,
 *         -1 on failure.
 */
int accept_client(int server_fd);


#endif