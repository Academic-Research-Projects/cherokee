#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/master/server_socket.h"

#define PORT 8080

void sigint_handler(int sig)
{
    printf("Received signal %d, shutting down...\n", sig);
    exit(0);
}

// main() function that starts the server
int main()
{
    signal(SIGINT, sigint_handler);
    fork_server(PORT);

    return 0;
}