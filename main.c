#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "crud_operations/http_get.h"

#define PORT 8080

int http_listen()
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections with a queue of 3
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // accept incoming connections and handle in separate threads
    int new_socket;
    struct sockaddr_in client_address;
    int addrlen = sizeof(client_address);
    pthread_t thread_id;
    while ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen)))
    {
        int *socket_desc = malloc(sizeof(*socket_desc));
        *socket_desc = new_socket;

        if (pthread_create(&thread_id, NULL, http_get, (void *)socket_desc) < 0)
        {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    // Création d'un thread pour l'écoute HTTP
    if (pthread_create(&tid, NULL, http_listen, NULL) != 0)
    {
        fprintf(stderr, "Erreur lors de la création du thread\n");
        return EXIT_FAILURE;
    }

    // Attente de la fin du thread
    if (pthread_join(tid, NULL) != 0)
    {
        fprintf(stderr, "Erreur lors de l'attente du thread\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}