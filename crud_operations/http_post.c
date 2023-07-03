#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "http/http_formatter/http_formatter.h"
#include "http/http_parser/http_parser.h"
#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include "status_codes/http_status_codes.h"

#define PORT 8080
#define BASE_DIRECTORY "test_files"
#define RESPONSE_BUFFER_SIZE 4096
#define FILE_PATH_SIZE 512
#define BUFFER_SIZE 1024

void *http_post(HttpRequest *request, int client_socket)
{
    char *request_body = request->body;

    // extract the requested file name from the client request
    char *request_target = request->request_line.requestTarget;
    char filename[256] = {0};
    sscanf(request_target, "/%s", filename);

    // construct the complete file path
    char file_path[FILE_PATH_SIZE] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, filename);

    // default content type
    char *content_type = "text/plain";
    struct HttpResponse *response = malloc(sizeof(struct HttpResponse));
    char *response_str;

    // check if the requested file is an HTML, JSON, JPEG, or PNG file
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

    if (access(file_path, F_OK) != -1)
    {
        response = createError409(response);
        response_str = format_http_response(response);
        write(client_socket, response_str, strlen(response_str));
    }
    else
    {
        int file_fd = open(file_path, O_RDWR | O_CREAT | S_IRWXU, 0644);
        if (file_fd == -1)
        {
            // file not found, send 404 response
            response = createError404(response);
            response_str = format_http_response(response);
            write(client_socket, response_str, strlen(response_str));
        }
        else
        {
            int status = write(file_fd, request_body, strlen(request_body));
            if (status == -1)
            {
                perror("write failed");
                return NULL;
            }

            createSuccess200(response, content_type);
            response_str = format_http_response(response);

            write(client_socket, response_str, strlen(response_str));

            free(response_str);
            free(response->headers);
            free(response);
            close(file_fd);
        }
    }

    close(client_socket);
    return NULL;
}