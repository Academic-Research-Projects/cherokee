#include <criterion/criterion.h>
#include "../master/server_socket.h"

Test(create_server_socket, returns_valid_socket_descriptor) {
    int port = 8080;
    int server_socket = create_server_socket(port);
    cr_assert(server_socket > 0, "Invalid socket descriptor");
}