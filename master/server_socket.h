#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

int create_server_socket(int port);

void fork_server(int *server_socket);

#endif