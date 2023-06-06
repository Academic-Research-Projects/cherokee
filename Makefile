CC=gcc
CFLAGS=-Wall -Wextra -pthread -g
TARGET=cherokee
SRCS=main.c crud_operations/http_get.c master/multiplex.c master/server_socket.c # liste des fichiers sources
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
