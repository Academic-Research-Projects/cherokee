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
    fork_server(PORT);

    return 0;
}