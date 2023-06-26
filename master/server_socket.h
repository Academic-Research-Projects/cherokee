#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

int create_server_socket(int port, int worker_id);

void fork_server(int port);

#endif