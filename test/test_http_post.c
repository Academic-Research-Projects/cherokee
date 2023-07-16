#include <criterion/criterion.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "http/http_request/http_request.h"
#include "http/http_response/http_response.h"
#include "http/http_parser/http_parser.h"
#include "http/http_formatter/http_formatter.h"
#include "http_post.h"

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

Test(http_post, test_create_file)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "POST";
    request->request_line.requestTarget = "/test.html";
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

    // call the http_post function
    http_post(request, client_socket);

    // check that the file was created
    char *file_path = "test_files/test.html";
    struct stat file_stats;
    int status = stat(file_path, &file_stats);
    cr_assert_eq(status, 0, "Expected file to be created");

    // delete the test file
    delete_test_file(file_path);
}

Test(http_post, test_create_file_with_content)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "POST";
    request->request_line.requestTarget = "/test.html";
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

    // call the http_post function
    http_post(request, client_socket);

    // check that the file was created with the correct content
    char *file_path = "test_files/test.html";
    char expected_content[] = "This is a test file.";
    char actual_content[1024];
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("File read error");
        exit(EXIT_FAILURE);
    }
    fgets(actual_content, sizeof(actual_content), file);
    fclose(file);
    cr_assert_str_eq(actual_content, expected_content, "Expected file content to be '%s'", expected_content);

    // delete the test file
    delete_test_file(file_path);
}

Test(http_post, test_create_file_with_existing_file)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "POST";
    request->request_line.requestTarget = "/test.html";
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
    char *file_path = "test_files/test.html";
    char *file_contents = "This is an existing test file.";
    create_test_file(file_path, file_contents);

    // call the http_post function
    http_post(request, client_socket);

    // check that the file was not modified
    char expected_content[] = "This is an existing test file.";
    char actual_content[1024];
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("File read error");
        exit(EXIT_FAILURE);
    }
    fgets(actual_content, sizeof(actual_content), file);
    fclose(file);
    cr_assert_str_eq(actual_content, expected_content, "Expected file content to be '%s'", expected_content);

    // delete the test file
    delete_test_file(file_path);
}

Test(http_post, test_create_file_with_invalid_extension)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "POST";
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

    // call the http_post function
    http_post(request, client_socket);

    // check that the response is a 415 Unsupported Media Type error
    char expected_response[] = "HTTP/1.1 415 Unsupported Media Type\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n";
    char actual_response[1024];
    int bytes_received = recv(client_socket, actual_response, sizeof(actual_response), 0);
    cr_assert_eq(bytes_received, strlen(expected_response), "Expected response length to be %d", strlen(expected_response));
    cr_assert_str_eq(actual_response, expected_response, "Expected response to be '%s'", expected_response);
}

Test(http_post, test_create_file_with_existing_file_error)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "POST";
    request->request_line.requestTarget = "/test.html";
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
    char *file_path = "test_files/test.html";
    char *file_contents = "This is an existing test file.";
    create_test_file(file_path, file_contents);

    // call the http_post function
    http_post(request, client_socket);

    // check that the response is a 409 Conflict error
    char expected_response[] = "HTTP/1.1 409 Conflict\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n";
    char actual_response[1024];
    int bytes_received = recv(client_socket, actual_response, sizeof(actual_response), 0);
    cr_assert_eq(bytes_received, strlen(expected_response), "Expected response length to be %d", strlen(expected_response));
    cr_assert_str_eq(actual_response, expected_response, "Expected response to be '%s'", expected_response);

    // delete the test file
    delete_test_file(file_path);
}

Test(http_post, test_create_file_with_file_creation_error)
{
    // create a valid HTTP request
    struct HttpRequest *request = malloc(sizeof(struct HttpRequest));
    request->method = "POST";
    request->request_line.requestTarget = "/test.html";
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

    // create a read-only directory
    char *dir_path = "test_files_readonly";
    mkdir(dir_path, 0444);

    // call the http_post function
    http_post(request, client_socket);

    // check that the response is a 500 Internal Server Error
    char expected_response[] = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n";
    char actual_response[1024];
    int bytes_received = recv(client_socket, actual_response, sizeof(actual_response), 0);
    cr_assert_eq(bytes_received, strlen(expected_response), "Expected response length to be %d", strlen(expected_response));
    cr_assert_str_eq(actual_response, expected_response, "Expected response to be '%s'", expected_response);

    // delete the test directory
    rmdir(dir_path);
}