CC=gcc
CFLAGS=-Wall -Wextra -pthread -g
LDLIBS = -lcriterion

TARGET=cherokee
TEST_TARGET = test_cherokee

SRCS=main.c crud_operations/http_get.c master/multiplex.c master/server_socket.c # liste des fichiers sources
OBJS=$(SRCS:.c=.o)
TEST_SRCS = $(wildcard test/*.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)

.PHONY: all clean test

all:$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) 

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $(TEST_TARGET) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)



filetest: $(TEST_TARGET)

test: filetest
	./$(TEST_TARGET) --verbose

ftest: test
	rm -f $(TEST_OBJS)
	rm -f $(TEST_TARGET)

fclean: clean
	rm -f $(TARGET)

re: fclean all
