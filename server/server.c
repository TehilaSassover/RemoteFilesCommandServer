#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include "../common/constants/constants.h"
#include "../common/types/types.h"
#include "../common/validation/validation.h"
#include "./files/file_manager.h"
#include "./socket/socket.h"
#include "./command/command_handler.h"

int main()
{
    const char *working_directory = "../storage";

    initialize_working_directory(working_directory);

    int server_fd = create_server_socket(SERVER_PORT);

    if (server_fd == -1)
    {
        return 1;
    }

    int client_fd = accept_client(server_fd);

    if (client_fd == -1)
    {
        close(server_fd);
        return 1;
    }

    handle_client_commands(client_fd, working_directory);

    close(client_fd);

    close(server_fd);


    return 0;
}