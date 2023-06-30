#include "include/master/server_socket.h"

#define PORT 8080

// main() function that starts the server
int main()
{
    fork_server(PORT);

    return 0;
}