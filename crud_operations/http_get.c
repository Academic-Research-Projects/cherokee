#include "http_response.h"
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

void *http_get(void *socket_desc)
{
    int client_socket = *(int *)socket_desc;

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

    // default content type
    char *content_type = "text/plain";

    // open the requested file
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        // file not found, send 404 response
        // HttpReponse error = createError404();
        char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found\r\n";
        write(client_socket, response, strlen(response));
    }
    else
    {
        // determine the content type based on the file extension
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
        }

        // send response headers
        dprintf(client_socket, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", content_type);

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
    close(client_socket);
    // free(socket_desc);

    return NULL;
}
