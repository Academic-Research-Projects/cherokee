/**
 * The function "handle_request" takes an HTTP request and a client socket as input, and based on the
 * request method, it calls the appropriate function to handle the request.
 * 
 * @param request A pointer to an instance of the HttpRequest struct, which contains information about
 * the HTTP request received from the client.
 * @param client_socket The `client_socket` parameter is the socket file descriptor for the client
 * connection. It is used to send the HTTP response back to the client.
 * 
 * @return a NULL pointer.
 */
#include "http/http_request/http_request.h"
#include "crud_operations/http_get.h"
#include "crud_operations/http_head.h"
#include "crud_operations/http_post.h"
#include "crud_operations/http_put.h"
#include "crud_operations/http_delete.h"
#include <stdio.h>
#include <string.h>

void *handle_request(HttpRequest *request, int client_socket)
{
    printf("Handle request : %s\n", request->request_line.method);
    if (strcmp(request->request_line.method, "GET") == 0)
    {
        printf("Http Get\n");
        // handle GET request
        http_get(request, client_socket);
    }
    else if (strcmp(request->request_line.method, "POST") == 0)
    {
        // handle POST request
        http_post(request, client_socket);
    }
    else if (strcmp(request->request_line.method, "PUT") == 0)
    {
        // handle PUT request
        http_put(request, client_socket);
    }
    else if (strcmp(request->request_line.method, "DELETE") == 0)
    {
        // handle DELETE request
        http_delete(request, client_socket);
    }
    else if (strcmp(request->request_line.method, "HEAD") == 0)
    {
        // handle DELETE request
        http_head(request, client_socket);
    }
    // else
    // {
    //     // handle invalid request
    //     // response = http_invalid_request();
    // }
    return NULL;
}