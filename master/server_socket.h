#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

int create_socket();

void bind_socket(int socket_fd, int port);

void listen_on_socket(int socket_fd);

int create_server_sockets(int port, int worker_id);

void fork_server(int port);

#endif