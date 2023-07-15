#ifndef WORKER_H
#define WORKER_H

#include <sys/epoll.h>

int create_epoll_instance();

struct epoll_event add_server_socket_to_epoll(int *server_socket);

int accept_client_connection(int *server_socket);

int worker(int *server_socket);

#endif