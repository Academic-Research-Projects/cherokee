CC=gcc
CFLAGS=-Wall -Wextra -pthread -g -Iinclude
TARGET=cherokee
SRCS=main.c \
crud_operations/http_get.c \
master/multiplex.c \
master/server_socket.c \
http/http_parser/http_parser.c \
http/http_formatter/http_formatter.c \
status_codes/status_codes_errors/http_client/error_404.c \
http/http_response/http_response.c \
http/http_handler/http_handler.c \
status_codes/status_codes_success/success_200.c \
crud_operations/http_head.c \
crud_operations/http_delete.c \
# crud_operations/http_post.c \
# crud_operations/http_put.c \

# http/http_request/http_request.c \
# http/http_request/headers/headers.c \
# http/http_request/request_line/request_line.c \
# http/http_response/http_response.c \
# status_codes/status_codes_errors/http_client/error_400.c \
# status_codes/status_codes_errors/http_client/error_401.c \
# status_codes/status_codes_errors/http_client/error_403.c \
# status_codes/status_codes_errors/http_client/error_404.c \
# status_codes/status_codes_errors/http_client/error_408.c \
# status_codes/status_codes_errors/http_client/error_429.c \
# status_codes/status_codes_errors/http_server/error_500.c \
# status_codes/status_codes_errors/http_server/error_501.c \
# status_codes/status_codes_errors/http_server/error_502.c \
# status_codes/status_codes_errors/http_server/error_503.c \
# status_codes/status_codes_errors/http_server/error_504.c \
# status_codes/status_codes_errors/http_server/error_505.c \
# status_codes/status_codes_redirections/redirection_301.c \
# status_codes/status_codes_redirections/redirection_302.c \
# status_codes/status_codes_redirections/redirection_304.c \
# status_codes/status_codes_success/success_200.c \
# status_codes/status_codes_success/success_201.c \
# status_codes/status_codes_success/success_204.c

OBJS=$(SRCS:.c=.o)

all:$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all