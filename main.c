#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 16

// thread that handles a client and return http response Hello World
void *cherokee_core(void *arg)
{
    int client_socket = *(int *)arg;

    // read request
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    printf("%s\n", buffer);

    // send response
    char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
    send(client_socket, response, strlen(response), 0);

    // close client socket
    close(client_socket);

    return NULL;
}

// Main thread that select all client sockets in a select loop and three threads to handle them in parallel
void *multiplex_connection(void *arg)
{
    int server_socket = *(int *)arg;

    // set server socket to non-blocking
    int flags = fcntl(server_socket, F_GETFL, 0);
    fcntl(server_socket, F_SETFL, flags | O_NONBLOCK);

    // initialize client sockets
    int client_sockets[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        client_sockets[i] = 0;
    }

    // initialize fd_set
    fd_set readfds;

    // clear fd_set
    FD_ZERO(&readfds);

    while (1)
    {
        // add server socket to fd_set
        FD_SET(server_socket, &readfds);

        // add client sockets to fd_set
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (client_sockets[i] > 0)
            {
                FD_SET(client_sockets[i], &readfds);
            }
        }

        // select sockets
        int max_sd = server_socket;
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (client_sockets[i] > max_sd)
            {
                max_sd = client_sockets[i];
            }
        }
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        // handle select error
        if ((activity < 0))
        {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        // handle server socket
        if (FD_ISSET(server_socket, &readfds))
        {
            // accept client socket
            int client_socket;
            if ((client_socket = accept(server_socket, NULL, NULL)) < 0)
            {
                perror("accept error");
                exit(EXIT_FAILURE);
            }

            // set client socket to non-blocking
            int flags = fcntl(client_socket, F_GETFL, 0);
            fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

            // add client socket to client sockets
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_sockets[i] == 0)
                {
                    client_sockets[i] = client_socket;
                    break;
                }
            }
        }
    }
}

// main() function that starts the server
int main()
{
    int server_socket;
    struct sockaddr_in address;
    int opt = 1;
    // int addrlen = sizeof(address);

    // create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set server socket options
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind server socket to address
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    multiplex_connection(&server_socket);

    return 0;
}