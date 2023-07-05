#include "../include/http/http_response/http_response.h"
#include "../include/http/http_request/http_request.h"
// #include "../include/status_codes/status_codes_errors/http_client/error_404.h"
#include "../include/status_codes/http_status_codes.h"
#include "../include/http/http_formatter/http_formatter.h"
#include "../include/http/http_parser/http_parser.h"
#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
// #include "status_codes/status_codes_errors/http_client/error_400.h"
// #include "status_codes/stauts_codes_success/success_200.h"
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
// #define FILE_PATH_SIZE 512
#define RESPONSE_BUFFER_SIZE 4096
// #define FILE_PATH_SIZE 512
// #define BUFFER_SIZE 1024

void *http_head(HttpRequest *request, int client_socket)
{
    // int client_socket = *(int *)socket_desc;

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
    int file_fd = open(file_path, O_RDONLY | O_WRONLY, 0666);
    printf("File descriptor: %d\n", file_fd);
    printf("File path: %s\n", file_path);

    if (file_fd == -1)
    {
        // File not found, send 404 response
        response = createError404(response);
        response_str = format_http_response(response);
        printf("Response: %s\n", response_str);
        write(client_socket, response_str, strlen(response_str));
        // write(client_socket, response->body, strlen(response->body));
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

        printf("%d\n", file_fd);
        puts("after open");

        if (file_fd == -1)
        {
            // File not found, send 404 response
            perror("open failed");
            response = createError404(response);
            response_str = format_http_response(response);
            printf("Response: %s\n", response_str);
            write(client_socket, response_str, strlen(response_str));
            // write(client_socket, response->body, strlen(response->body));
        }
        else
        {
            if (strcmp(file_extension, ".txt") == 0)
            {
                printf("Test content file txt.\n");
                // send response headers
                char response[RESPONSE_BUFFER_SIZE];
                snprintf(response, RESPONSE_BUFFER_SIZE, "HTTP/1.1 204 OK\r\nContent-Type: %s\r\nContent-Length: %s\r\nLast-Modified: %s\r\nETag: %s\r\n\r\n", content_type, "future content length", "future last modified", "future ETag");
                write(client_socket, response, strlen(response));
            }
        }

        // // read client request
        // char buffer[1024] = {0};                            // string array
        // ssize_t client_request;                             // signed integer
        // client_request = read(client_socket, buffer, 1024); // nombre de bytes dans le buffer du file descriptor
        // printf("%ld\n", client_request);
        // if (client_request < 0)
        // {
        //     perror("read error");
        // }
        // printf("%s\n", buffer);

        // // extract the requested file name from the client request
        // char filename[256] = {0};
        // sscanf(buffer, "HEAD /%s", filename);

        // // construct the complete file path
        // char file_path[FILE_PATH_SIZE] = {0};
        // snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, filename); // snpritnf is used to write formatted data to a character array (string) with a specified maximum lentgh

        // // default content type
        // char *content_type = "text/plain"; // The MIME type "text/plain" is commonly used for files or data that contain plain text without any specific formatting or markup.

        // // check if the requested file is an HTML, JSON, JPEG, or PNG file
        // char *file_extension = strrchr(filename, '.'); // strrchrr cherche la dernière occurrence définie dans le deuxième argument
        // if (file_extension)
        // {
        //     if (strcmp(file_extension, ".html") == 0)
        //         content_type = "text/html";
        //     else if (strcmp(file_extension, ".json") == 0)
        //         content_type = "application/json";
        //     else if (strcmp(file_extension, ".jpeg") == 0 || strcmp(file_extension, ".jpg") == 0)
        //         content_type = "image/jpeg";
        //     else if (strcmp(file_extension, ".png") == 0)
        //         content_type = "image/png";
        //     else if (strcmp(file_extension, ".txt") == 0)
        //         content_type = "text/txt";
        // }

        // if (client_request > 0)
        // {
        //     int file_fd = open(file_path, O_RDONLY | O_WRONLY, 0666); // 0_(...) are flags that represent opening mode and options for the file
        //     printf("%d\n", file_fd);
        //     puts("after open");
        //     if (file_fd == -1) // error : FD cannot open
        //     {
        //         puts("error");
        //         // file not found, send 404 response
        //         perror("open failed");
        //         char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found, the path doesn't exist\r\n";
        //         write(client_socket, not_found_response, strlen(not_found_response));
        //     }
        //     else
        //     {
        //         if (strcmp(file_extension, ".txt") == 0)
        //         {
        //             printf("Test content file txt.\n");
        //             // send response headers
        //             // char response[RESPONSE_BUFFER_SIZE];
        //             // snprintf(response, RESPONSE_BUFFER_SIZE, "HTTP/1.1 204 OK\r\nContent-Type: %s\r\nContent-Length: %s\r\nLast-Modified: %s\r\nETag: %s\r\n\r\n", content_type, "future content length", "future last modified", "future ETag");
        //             // write(client_socket, response, strlen(response));
        //         }
        //     }
        // }
    }
    close(client_socket);
    return NULL;
}