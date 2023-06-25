#include "../include/http/http_response/http_response.h"
#include "../include/http/http_request/http_request.h"
#include "../include/status_codes/status_codes_errors/http_client/error_404.h"
#include "../include/http/http_formatter/http_formatter.h"
#include "../include/http/http_parser/http_parser.h"
#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include "status_codes/status_codes_errors/http_client/error_400.h"
#include "status_codes/stauts_codes_success/success_200.h"
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

void *http_get(HttpRequest *request, int client_socket)
{

    // read client request
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    printf("%s\n", buffer);

    // extract the requested file name from the client request
    char filename[256] = {0};
    sscanf(buffer, "GET /%s", filename);

    // construct the complete file path
    char file_path[512] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, filename);

    // print request
    printf("Request: %s\n", request->request_line.requestTarget);

    // default content type
    char *content_type = "text/plain";

    // struct HttpResponse *response;
    struct HttpResponse *response = malloc(sizeof(struct HttpResponse));
    char *response_str;
    // printf("Response: %s\n", response_str);

    // open the requested file
    int file_fd = open(file_path, O_RDONLY);
    printf("File descriptor: %d\n", file_fd);
    printf("File path: %s\n", file_path);
    printf("Before if condition\n");
    if (file_fd == -1)
    {
        printf("Inside if condition\n");
        // file not found, send 404 response
        // createNotFoundError(response);
        response = createError404(response);
        response_str = format_http_response(response);
        printf("Response: %s\n", response_str);
        write(client_socket, response_str, strlen(response_str));
        // free(response_str);
    }
    else
    {
        // determine the content type based on the file extension
        char *file_extension = strrchr(filename, '.');
        // char *content_type __attribute__((unused)) = "text/plain";
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

        // createOkSuccess(response);
        createSuccess200(response, content_type);
        response_str = format_http_response(response);

        write(client_socket, response_str, strlen(response_str));
        free(response_str);

        // read and send the file contents
        char file_buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0)
        {
            write(client_socket, file_buffer, bytes_read);
        }

        close(file_fd);
    }

    // close socket and free memory
    // close(client_socket);
    // free(socket_desc);

    return NULL;
}
