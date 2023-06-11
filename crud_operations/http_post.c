#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
#define BASE_DIRECTORY "test_files"
#define RESPONSE_BUFFER_SIZE 4096
#define FILE_PATH_SIZE 512
#define BUFFER_SIZE 1024

void *http_post(void *socket_desc)
{
    int client_socket = *(int *)socket_desc;
    printf("%d\n", client_socket);

    // read client request
    // buffer c'est un string (tableau)
    char buffer[1024] = {0};
    ssize_t client_request;
    // nombre de bytes dans le buffer du file descriptor
    client_request = read(client_socket, buffer, 1024);
    printf("%ld\n", client_request);
    if (client_request < 0)
    {
        perror("read");
    }
    printf("%s\n", buffer);

    // extract the requested file name from the client request
    // filename c'est un tableau
    char filename[256] = {0};
    sscanf(buffer, "POST /%s", filename);

    // construct the complete file path
    char file_path[FILE_PATH_SIZE] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, filename);

    // default content type
    char *content_type = "text/plain";

    // check if the requested file is an HTML, JSON, JPEG, or PNG file
    // strrchrr cherche la dernière occurrence définie dans le deuxième argument
    char *file_extension = strrchr(filename, '.');
    if (file_extension)
    {
        if (strcmp(file_extension, ".html") == 0)
            content_type = "text/html";
        else if (strcmp(file_extension, ".json") == 0)
            content_type = "application/json";
        else if (strcmp(file_extension, ".jpeg") == 0 || strcmp(file_extension, ".jpg") == 0)
            content_type = "image/jpeg";
        else if (strcmp(file_extension, ".png") == 0)
            content_type = "image/png";
        else if (strcmp(file_extension, ".txt") == 0)
            content_type = "text/txt";
    }

    // read the request body
    // char request_body[1024] = {0};
    // ssize_t bytes_read;
    // ouvrir le fichier
    puts("hello ge");
    // bytes_read = read(client_socket, request_body, sizeof(request_body));

    if (client_request > 0)
    {
        // Null-terminate the buffer to treat it as a string
        // le dernier byte de buffer c'est un "\0"
        buffer[client_request] = '\0';
        // h-o-l-a-\0
        // example : buffer[4]

        // Process the headers
        // line c'est un tableau
        // strtok return un pointeur vers le premier token
        //     char *line = strtok(buffer, "\r\n");
        //     while (line != NULL)
        //     {
        //         // Print or process each header line
        //         printf("%s\n", line);

        //         // Move to the next line
        //         // strtok() "mange" le deuxième retour à la ligne
        //         line = strtok(NULL, "\r\n");
        //     }
        // }

        char *request_body = strstr(buffer, "\r\n\r\n") + 4;
        int file_fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC);
        printf("%d\n", file_fd);
        puts("after open");
        if (file_fd == -1)
        {
            puts("error");
            // file not found, send 404 response
            perror("open failed");
            char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found\r\n";
            write(client_socket, not_found_response, strlen(not_found_response));
        }
        else
        {
            if (!request_body)
            {
                puts("aja");
            }
            int status = write(file_fd, request_body, strlen(request_body));
            printf("%d\n", status);
            if (status == -1)
            {
                perror("write failed");
                return NULL;
            }
            // send response headers
            char response[RESPONSE_BUFFER_SIZE];
            snprintf(response, RESPONSE_BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n%s", content_type, "Your request have been posted\n");
            write(client_socket, response, strlen(response));
            close(file_fd);
        }

        // while ((bytes_read = read(client_socket, request_body, sizeof(request_body))) > 0)
        // {
        //     printf("request : %s\n", buffer);
        //     // process the request body (you can add your own logic here)
        //     /////////////////////////////////////////////////////////////////////////////////
        //     // lire le fichier
        //     //
        //     printf("Received request body: %s\n", request_body);
        // }

        // open and send the requested file contents
        // int file_fd = open(file_path, O_RDONLY);

        // close socket and free memory
        close(client_socket);
        // free(socket_desc);
    }
    return NULL;
}