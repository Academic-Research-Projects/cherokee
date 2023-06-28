#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../include/http/http_response/http_response.h"
#include "../include/http/http_request/http_request.h"
#include "../include/status_codes/status_codes_errors/http_client/error_404.h"
#include "../include/http/http_formatter/http_formatter.h"
#include "../include/http/http_parser/http_parser.h"
#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include "status_codes/status_codes_errors/http_client/error_400.h"
#include "status_codes/stauts_codes_success/success_200.h"

#define PORT 8080
#define BASE_DIRECTORY "test_files"
#define RESPONSE_BUFFER_SIZE 4096
#define FILE_PATH_SIZE 512
#define BUFFER_SIZE 1024

void *http_post(HttpRequest *request, int client_socket)
{
    // extract the file descriptor (socket descriptor) of the client connection from a void pointer.
    int client_socket = *(int *)socket_desc;
    printf("%d\n", client_socket);

    // read client request
    char buffer[1024] = {0}; // buffer c'est un string (tableau)
    ssize_t client_request;
    client_request = read(client_socket, buffer, 1024); // nombre de bytes dans le buffer du file descriptor
    printf("%ld\n", client_request);
    if (client_request < 0)
    {
        perror("read error");
    }
    printf("%s\n", buffer);

    // extract the requested file name from the client request
    char filename[256] = {0};             // filename c'est un tableau
    sscanf(buffer, "POST /%s", filename); // sscanf() parse data from a string and store the extracted value into variables

    // construct the complete file path
    char file_path[FILE_PATH_SIZE] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, filename);

    // default content type
    char *content_type = "text/plain";
    struct HttpResponse *response = malloc(sizeof(struct HttpResponse));
    char *response_str;

    // check if the requested file is an HTML, JSON, JPEG, or PNG file
    char *file_extension = strrchr(filename, '.'); // strrchrr cherche la dernière occurrence définie dans le deuxième argument
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

    puts("hello ge");

    if (access(file_path, F_OK) != -1)
    {
        char *not_found_response = "HTTP/1.1 409 Conflict\r\nContent-Type: text/plain\r\n\r\nThe file already exists\r\n";
        write(client_socket, not_found_response, strlen(not_found_response));
    }
    else
    {
        // read the request body
        if (client_request > 0)
        {
            // Null-terminate the buffer to treat it as a string
            buffer[client_request] = '\0';

            char *request_body = strstr(buffer, "\r\n\r\n") + 4;
            int file_fd = open(file_path, O_RDWR | O_CREAT | S_IRWXU, 0644);
            printf("%d\n", file_fd);
            puts("after open");
            if (file_fd == -1)
            {
                puts("error");
                // file not found, send 404 response
                perror("open failed");
                response = createError404(response);
                response_str = format_http_response(response);
                printf("Response: %s\n", response_str);
                write(client_socket, response_str, strlen(response_str));
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
        }
    }
    // close socket and free memory
    close(client_socket);
    // free(socket_desc);
    return NULL;
}