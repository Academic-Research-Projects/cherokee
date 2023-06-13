#include "http_response.h"
#include "http_request.h"

void handle_request(HttpRequest *request, int clientSocket)
{

    char *responseLine = "HTTP/1.1 200 OK\r\n";
    HttpResponse response;
    response.status_code = 200;
    response.content_type = "text/plain";
    response.content_length = 0;
    response.content = NULL;

    // construct the complete file path
    char file_path[512] = {0};
    snprintf(file_path, sizeof(file_path), "%s/%s", BASE_DIRECTORY, request->path);

    // open the requested file
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        // file not found, send 404 response
        response.status_code = 404;
        response.content = "File not found";
    }
    else
    {
        // determine the content type based on the file extension
        char *file_extension = strrchr(request->path, '.');
        if (file_extension)
        {
            if (strcmp(file_extension, ".html") == 0)
                response.content_type = "text/html";
            else if (strcmp(file_extension, ".json") == 0)
                response.content_type = "application/json";
            else if (strcmp(file_extension, ".jpeg") == 0 || strcmp(file_extension, ".jpg") == 0)
                response.content_type = "image/jpeg";
            else if (strcmp(file_extension, ".png") == 0)
                response.content_type = "image/png";
        }

        // read the file contents
        char file_buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0)
        {
            response.content_length += bytes_read;
            response.content = realloc(response.content, response.content_length);
            memcpy(response.content + response.content_length - bytes_read, file_buffer, bytes_read);
        }

        close(file_fd);
    }

    return httpResponse;
}