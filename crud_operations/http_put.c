#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include "http/http_formatter/http_formatter.h"
#include "http/http_parser/http_parser.h"
#include "status_codes/http_status_codes.h"

#define PORT 8080
#define BASE_DIRECTORY "test_files"
#define FILE_PATH_SIZE 512
#define RESPONSE_BUFFER_SIZE 4096
#define FILE_PATH_SIZE 512
#define BUFFER_SIZE 1024

void *http_put(HttpRequest *request, int client_socket)
{
    // Extract the request target from the request line
    char *request_target = request->request_line.requestTarget;
    char *request_body = request->body;

    // Construct the complete file path
    char file_path[FILE_PATH_SIZE] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, request_target);

    struct HttpResponse *response = malloc(sizeof(struct HttpResponse));
    char *response_str;

    // Determine the content type based on the file extension
    char *content_type = "text/plain"; // default to plain text
    char *file_extension = strrchr(request_target, '.');
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

    // Open the file in write-only mode, creating it if it doesn't exist
    int file_fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1)
    {
        // An error occurred while trying to open the file
        perror("open failed");
        response = createError500(response);
        response_str = format_http_response(response);
        write(client_socket, response_str, strlen(response_str));
        free(response_str);
        free(response->headers);
        free(response);
    }
    else
    {
        // Successfully opened the file, write the request body to it
        ssize_t status = write(file_fd, request_body, strlen(request_body));
        if (status == -1)
        {
            // An error occurred while trying to write to the file
            perror("write failed");
            close(file_fd);
            return NULL;
        }

        // Successfully wrote to the file, send a 200 OK response
        close(file_fd);
        createSuccess200(response, content_type);
        response_str = format_http_response(response);
        write(client_socket, response_str, strlen(response_str));
        free(response_str);
        free(response->headers);
        free(response);
    }

    // Close the client socket
    close(client_socket);
    return NULL;
}
