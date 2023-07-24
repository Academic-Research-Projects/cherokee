/**
 * The function `http_head` handles a HEAD request by opening the requested file, determining its
 * content type, and sending the appropriate response headers.
 *
 * @param request The parameter "request" is a pointer to an instance of the HttpRequest struct. It
 * contains information about the HTTP request, such as the request line, headers, and request target.
 * @param client_socket The `client_socket` parameter is the file descriptor for the socket connection
 * between the server and the client. It is used to send the HTTP response back to the client.
 *
 * @return a NULL pointer.
 */
#include "../include/http/http_response/http_response.h"
#include "../include/http/http_request/http_request.h"
#include "../include/status_codes/http_status_codes.h"
#include "../include/http/http_formatter/http_formatter.h"
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

#define BASE_DIRECTORY "test_files"
#define RESPONSE_BUFFER_SIZE 4096

void *http_head(HttpRequest *request, int client_socket)
{
    // Get the requested file name from the HttpRequest
    char *request_target = request->request_line.requestTarget;

    // Construct the complete file path
    char file_path[512] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, request_target);

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
        free(response_str);
        free(response->headers);
        free(response);
    }
    else
    {
        // Determine the content type based on the file extension
        char *file_extension = strrchr(request_target, '.');

        if (file_fd == -1)
        {
            // File not found, send 404 response
            perror("open failed");
            response = createError404(response);
            response_str = format_http_response(response);
            write(client_socket, response_str, strlen(response_str)); 
        }
        else
        {
            if (strcmp(file_extension, ".txt") == 0)
            {
               
                response = createSuccess204(response);
                response_str = format_http_response(response);
                write(client_socket, response_str, strlen(response_str));
            }
        }
        free(response_str);
        free(response->headers);
        free(response);
    }
    close(client_socket);
    return NULL;
}