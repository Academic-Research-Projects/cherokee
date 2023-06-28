
            char *request_body = strstr(buffer, "\r\n\r\n") + 4;
            int file_fd = open(file_path, O_RDWR | O_CREAT | S_IRWXU, 0644);
            printf("%d\n", file_fd);
            puts("after open");
            if (file_fd == -1)
            {
                puts("error");
                // file not found, send 404 response
                perror("open failed");
                char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found\r\n";
                write(client_socket, not_found_response, strlen(not_found_response));
            }
            else
            {
                if (!request_body)
                {
                    puts("aja");
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
                snprintf(response, RESPONSE_BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n%s", content_type, "Your request have been posted\n");
                write(client_socket, response, strlen(response));
                close(file_fd);
            }
        }
    }
    // close socket and free memory
    close(client_socket);
    // free(socket_desc);
    return NULL;
}