#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include "../common/constants/constants.h"
#include "../common/types/types.h"
#include "../common/validation/validation.h"
#include "./socket/socket.h"
#include "./input/input.h"

int main()
{
    int client_fd = connect_to_server(SERVER_IP, SERVER_PORT);

    if (client_fd == -1)
    {
        return 1;
    }

    run_client(client_fd);

    close(client_fd);

    return 0;
}