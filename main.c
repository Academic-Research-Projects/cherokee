#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "master/multiplex.h"
#include "master/server_socket.h"
#include "crud_operations/http_get.h"

#define PORT 8080

// main() function that starts the server
int main()
{
    int server_socket = create_server_socket(PORT);

    fork_server(&server_socket);

    // multiplex_connections(&server_socket);

    return 0;
}