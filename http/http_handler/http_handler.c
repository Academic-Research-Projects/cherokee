#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include "crud_operations/http_get.h"
#include <stdio.h>

void handle_request(HttpRequest *request, int client_socket)
{
    printf("Handling request from method");
    if (strcmp(request->request_line.method, "GET") == 0)
    {
        printf("Http Get");
        // handle GET request
        http_get(request, client_socket);
    }
    // else if (strcmp(request->request_line.method, "POST") == 0)
    // {
    //     // handle POST request
    //     http_post(request, client_socket);
    // }
    // else if (strcmp(request->request_line.method, "PUT") == 0)
    // {
    //     // handle PUT request
    //     http_put(request, client_socket);
    // }
    // else if (strcmp(request->request_line.method, "DELETE") == 0)
    // {
    //     // handle DELETE request
    //     http_delete(request, client_socket);
    // }
    // else
    // {
    //     // handle invalid request
    //     // response = http_invalid_request();
    // }
}