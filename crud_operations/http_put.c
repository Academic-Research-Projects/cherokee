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
#define FILE_PATH_SIZE 512
#define RESPONSE_BUFFER_SIZE 4096
#define FILE_PATH_SIZE 512
#define BUFFER_SIZE 1024

void *http_put(void *socket_desc)
{

    // extract the file descriptor (socket descriptor) of the client connection from a void pointer.
    int client_socket = *(int *)socket_desc; // file descriptor
    printf("%d\n", client_socket);

    // read client request
    char buffer[1024] = {0};                            // string array
    ssize_t client_request;                             // signed integer
    client_request = read(client_socket, buffer, 1024); // nombre de bytes dans le buffer du file descriptor
    printf("%ld\n", client_request);
    if (client_request < 0)
    {
        perror("read error");
    }
    printf("%s\n", buffer);

    // extract the requested file name from the client request
    char filename[256] = {0};            // filename c'est un tableau
    sscanf(buffer, "PUT /%s", filename); // sscanf() parse data from a string and store the extracted value into variables

    // construct the complete file path
    char file_path[FILE_PATH_SIZE] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, filename); // snpritnf is used to write formatted data to a character array (string) with a specified maximum lentgh

    // default content type
    char *content_type = "text/plain"; // The MIME type "text/plain" is commonly used for files or data that contain plain text without any specific formatting or markup.

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

    // read the request body and modify it
    if (client_request > 0)
    {
        // Null-terminate the buffer to treat it as a string
        buffer[client_request] = '\0';

        char *request_body = strstr(buffer, "\r\n\r\n") + 4;
        int file_fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644); // | S_IRUSR | S_IWUSR); // 0_(...) are flags that represent opening mode and options for the file
        printf("%d\n", file_fd);
        puts("after open");
        if (file_fd == -1) // error : FD cannot open
        {
            puts("error");
            // file not found, send 404 response
            perror("open failed");
            char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found, the path doesn't exist\r\n";
            write(client_socket, not_found_response, strlen(not_found_response));
        }
        else
        {
            if (!request_body)
            {
                puts("there isn't a request body");
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
            snprintf(response, RESPONSE_BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n%s", content_type, "Your request have been modified\n");
            write(client_socket, response, strlen(response));
            close(file_fd);
        }
    }
    close(client_socket);
    return NULL;
}