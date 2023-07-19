CC=gcc
CFLAGS=-Wall -Wextra -pthread -g -Iinclude
LDLIBS=-lcriterion
TARGET=cherokee
TEST_TARGET = test_cherokee

# liste des fichiers sources
SRCS=main.c \
$(wildcard master/*.c) \
$(wildcard crud_operations/*.c) \
$(wildcard http/*/*.c) \
$(wildcard status_codes/status_codes_success/*.c) \
$(wildcard status_codes/status_codes_errors/*/*.c) \
status_codes/http_status_codes.c

OBJS=$(SRCS:.c=.o)
TEST_SRCS = $(wildcard test/*.c) \
$(wildcard master/*.c) \
$(wildcard crud_operations/*.c) \
$(wildcard http/*/*.c) \
status_codes/status_codes_success/success_200.c \
status_codes/status_codes_errors/http_client/error_404.c

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