#include <criterion/criterion.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "http/http_request/http_request.h"
#include "http/http_response/http_response.h"
#include "http/http_parser/http_parser.h"
#include "http/http_formatter/http_formatter.h"
#include "http/http_put/http_put.h"

#define PORT 8080
#define BASE_DIRECTORY "test_files"

void create_test_file(char *file_path, char *file_contents)
{
    FILE *file = fopen(file_path, "w");
    if (file == NULL)
    {
        perror("File creation error");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", file_contents);
    fclose(file);
}

void delete_test_file(char *file_path)
{
    remove(file_path);
}

Test(http_put, test_create_text_file)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "PUT";
    request->request_line.requestTarget = "/test.txt";
    request->body = "This is a test file.";

    // create a mock client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    int connection_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    // call the http_put function
    http_put(request, client_socket);

    // check that the file was created with the correct contents
    char *file_path = "test_files/test.txt";
    char buffer[1024];
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        perror("File open error");
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_read = read(file_fd, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("File read error");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    cr_assert_str_eq(buffer, "This is a test file.", "Expected file contents to be 'This is a test file.'");

    // delete the test file
    delete_test_file(file_path);
}

Test(http_put, test_create_html_file)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "PUT";
    request->request_line.requestTarget = "/test.html";
    request->body = "<html><body><h1>Hello, world!</h1></body></html>";

    // create a mock client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    int connection_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    // call the http_put function
    http_put(request, client_socket);

    // check that the file was created with the correct contents
    char *file_path = "test_files/test.html";
    char buffer[1024];
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        perror("File open error");
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_read = read(file_fd, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("File read error");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    cr_assert_str_eq(buffer, "<html><body><h1>Hello, world!</h1></body></html>", "Expected file contents to be '<html><body><h1>Hello, world!</h1></body></html>'");

    // delete the test file
    delete_test_file(file_path);
}

Test(http_put, test_create_json_file)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "PUT";
    request->request_line.requestTarget = "/test.json";
    request->body = "{\"name\": \"John\", \"age\": 30, \"city\": \"New York\"}";

    // create a mock client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    int connection_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    // call the http_put function
    http_put(request, client_socket);

    // check that the file was created with the correct contents
    char *file_path = "test_files/test.json";
    char buffer[1024];
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        perror("File open error");
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_read = read(file_fd, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("File read error");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    cr_assert_str_eq(buffer, "{\"name\": \"John\", \"age\": 30, \"city\": \"New York\"}", "Expected file contents to be '{\"name\": \"John\", \"age\": 30, \"city\": \"New York\"}'");

    // delete the test file
    delete_test_file(file_path);
}

Test(http_put, test_create_image_file)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "PUT";
    request->request_line.requestTarget = "/test.jpg";
    request->body = "This is an image file.";

    // create a mock client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    int connection_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    // call the http_put function
    http_put(request, client_socket);

    // check that the file was created with the correct contents
    char *file_path = "test_files/test.jpg";
    char buffer[1024];
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        perror("File open error");
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_read = read(file_fd, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("File read error");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    cr_assert_str_eq(buffer, "This is an image file.", "Expected file contents to be 'This is an image file.'");

    // delete the test file
    delete_test_file(file_path);
}

Test(http_put, test_create_existing_file)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "PUT";
    request->request_line.requestTarget = "/test.txt";
    request->body = "This is a test file.";

    // create a mock client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    int connection_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    // create an existing test file
    char *file_path = "test_files/test.txt";
    char *file_contents = "This is an existing test file.";
    create_test_file(file_path, file_contents);

    // call the http_put function
    http_put(request, client_socket);

    // check that the file was not overwritten
    char buffer[1024];
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        perror("File open error");
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_read = read(file_fd, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("File read error");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    cr_assert_str_eq(buffer, file_contents, "Expected file contents to be '%s'", file_contents);

    // delete the test file
    delete_test_file(file_path);
}