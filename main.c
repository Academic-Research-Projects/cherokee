#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/master/server_socket.h"

#define PORT 8080

// main() function that starts the server
int main()
{
    // Fork the server process
    fork_server(PORT);

    return 0;
}