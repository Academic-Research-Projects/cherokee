#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include "http/http_formatter/http_formatter.h"
#include "http/http_parser/http_parser.h"
#include "status_codes/http_status_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BASE_DIRECTORY "test_files"

void *http_get(HttpRequest *request, int client_socket)
{
    // Get the requested file name from the HttpRequest
    char *request_target = request->request_line.requestTarget;

    // Construct the complete file path
    char file_path[512] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, request_target);

    // Print request
    printf("Request: %s\n", request_target);

    // Default content type
    char *content_type = "text/plain";

    struct HttpResponse *response = malloc(sizeof(struct HttpResponse));
    char *response_str;

    // Open the requested file
    int file_fd = open(file_path, O_RDONLY);
    printf("File descriptor: %d\n", file_fd);
    printf("File path: %s\n", file_path);

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
        }

        createSuccess200(response, content_type);
        response_str = format_http_response(response);

        printf("Response: %s\n", response_str);

        write(client_socket, response_str, strlen(response_str));

        free(response_str);
        free(response->headers);
        free(response);

        // Read and send the file contents
        char file_buffer[1024];
        ssize_t bytes_read;

        printf("Writing request to client socket");

        while ((bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0)
        {
            write(client_socket, file_buffer, bytes_read);
        }

        close(file_fd);
    }

    // close socket and free memory
    // close(client_socket);
    // free(socket_desc);

    close(client_socket);
    return NULL;
}
