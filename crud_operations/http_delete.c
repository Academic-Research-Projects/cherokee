/**
 * The function `http_delete` handles a DELETE request by deleting the requested file and sending an
 * appropriate response.
 *
 * @param request The "request" parameter is a pointer to an instance of the "HttpRequest" struct. This
 * struct contains information about the HTTP request, such as the request line, headers, and body. It
 * is used to extract the requested file name from the request.
 * @param client_socket The `client_socket` parameter is the file descriptor for the socket connection
 * between the server and the client. It is used to send the HTTP response back to the client.
 *
 * @return NULL.
 */
#include "../include/http/http_response/http_response.h"
#include "../include/http/http_request/http_request.h"
#include "../include/http/http_formatter/http_formatter.h"
#include "../include/status_codes/http_status_codes.h"
#include "../include/http/http_parser/http_parser.h"
#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// #define PORT 8080
#define BASE_DIRECTORY "test_files"

void *http_delete(HttpRequest *request, int client_socket)
{

    // Get the requested file name from the HttpRequest
    char *request_target = request->request_line.requestTarget;

    // Construct the complete file path
    char file_path[512] = {0};
    strcpy(file_path, BASE_DIRECTORY);
    strcat(file_path, request_target);

    // Default content type
    char *content_type = "text/plain";

    struct HttpResponse *response = malloc(sizeof(struct HttpResponse));
    char *response_str;

    // Open the requested file
    int file_fd = open(file_path, O_RDONLY | O_WRONLY, 0666);

    if (file_fd == -1)
    {
        // File not found, send 404 response
        response = createError404(response);
        response_str = format_http_response(response);
        write(client_socket, response_str, strlen(response_str));
    }
    else
    {
        // Determine the content type based on the file extension
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

        if (remove(file_path) == 0)
        {
            // send response headers
            createSuccess200(response, content_type);
            response_str = format_http_response(response);
            write(client_socket, response_str, strlen(response_str));
        }
        else
        {
            perror("Error deleting file");
        }
        close(file_fd);
    }
    close(client_socket);
    free(response->body);
    free(response);
    return NULL;
}