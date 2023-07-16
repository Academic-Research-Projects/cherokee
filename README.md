# Groupe de blanch_b 1003936
login: blanch_b loizag_g utzman_f baudet_c

# Cherokee - HTTP Server Implementation in C

## Overview:
The Cherokee project aims to implement a simplified version of the HTTP protocol in C, creating a server capable of serving files and handling client requests. The project focuses on networking, understanding sockets and the TCP protocol, as well as designing architectures that meet performance requirements.

## Key Objectives:

- Communicating with Clients: The server communicates with clients using TCP sockets and handles requests asynchronously to serve multiple clients simultaneously.
- Implementing the HTTP Protocol: The server partially implements the HTTP 1.1 protocol, including parsing headers, supporting methods like GET, HEAD, POST, PUT, and DELETE, and generating appropriate responses.
- Serving Static Files: The server can serve various file types, such as text files, HTML files, JSON files, and different image formats, with the correct content type.
- Generating Customizable Responses: In addition to serving static files, the server can associate specific URLs or patterns with predefined logic (e.g., CRUD operations), allowing for customizable responses.
- Architecturing the Project: It is essential to design the project's architecture before implementation, considering aspects like data reception from clients, handling disconnections, managing invalid or incomplete requests, and handling multiple requests simultaneously.
- Performance Considerations: Performance is a crucial aspect of the project, requiring the identification of critical performance factors such as CPU performance, memory management, and I/O operations. Optimizing these aspects is necessary to ensure efficient server performance.
- Testing the Project: Thorough testing is essential, including unit tests to validate individual code portions and functional tests to verify overall project behavior.
- Demonstration: During the project presentation, justifications for architectural choices and algorithms will be provided, along with demonstrations using benchmarks to showcase performance improvements.
- External Libraries: While the project primarily relies on the C standard library and POSIX threads, the use of additional libraries is allowed if they do not implement project logic and can be justified in terms of relevance and integration into the architecture.

By successfully completing the Cherokee project, you will gain a deeper understanding of networking concepts, socket communication, the HTTP protocol, and designing performant server architectures in C.

## Requirements
- Linux (Tested on Debian 11)
- GCC compiler : `sudo apt-get install build-essential`
- Apache Bench : `sudo apt install apache2-utils`
    - Usage : `ab -n 1000 -c 100 http://localhost:8080/`
- Valgrind : `sudo apt-get install valgrind`
- GDB : `sudo apt-get install gdb`
- Criterion : https://github.com/Snaipe/Criterion#downloads 
    - Ubuntu (>=21.04) / Debian (>=11) : `sudo apt-get install libcriterion-dev`
    - Other : https://criterion.readthedocs.io/en/latest/setup.html#installation
- Siege : `sudo apt-get install siege`

## How to use
- Compile the program : `make`
- Run the program : `./cherokee`
- Clean the program : `make clean`
- Clean the program and the executable : `make fclean`
- Re-compile the program : `make re`
- Run the program with valgrind : `valgrind ./cherokee` => Debug mode (memory leaks)
- Run the program with gdb : `gdb ./cherokee` => Debug mode
- Run apache bench : `ab -n 1000 -c 100 http://localhost:8080/`
- Run criterion tests : `make ftest`
- Run siege :
    - over time : `siege -c 100 -t10s '127.0.0.1:8080/index.html'`
    - over number of requests : `siege -c 100 -r100 '127.0.0.1:8080/index.html'`

## How to test the HTTP GET request, asking for a .json file
- Open another terminal
- Execute `curl http://localhost:8080/zoubisou.json`
- You should see a "json-formatted file" in the client terminal
- This is the output expected from the server, in your server terminal :

`GET / HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.74.0
Accept: */*`

`The zou-bisou.json file`

## How to test the HTTP POST request, posting a .txt file
- Open another terminal
- Execute `curl -X POST -d "hello antho" http://localhost:8080/file.txt`
- You should see the text in your server terminal

## How to test the HTTP PUT request, modifying or adding a .txt file
- Open another terminal
- Execute `curl -X PUT -d "this file has been modified" http://localhost:8080/file.txt`
- You should see the text in your server terminal

## How to test the HTTP DELETE request, deleting a .txt file
- Open another terminal
- Execute `curl -X DELETE http://localhost:8080/file-to-erase.txt`
- You should see the text in your server terminal

## How to test the HTTP HEAD request, deleting a .txt file
- Open another terminal
- Execute `curl -I -X HEAD http://localhost:8080/sendhelp.txt`
- You should see the text in your server terminal

## How to make Criterion unit tests
- Create a file named `test_*.c` in the `test` folder or use an existing one for the same purpose
- Write your tests in this file
    - Include the criterion header : `#include <criterion/criterion.h>`
    - Include the header of the file you want to test : `#include "../src/*.h"`
    - Write a function named : `Test(TestName, TestDescription)`
    - Write your tests in this function
    - Use the criterion macros to test your functions :
        - `cr_assert(condition)` : Assert that the condition is true
        - documentation : https://criterion.readthedocs.io/en/latest/assert.html
- Group tests by purpose/features in different files
- You can use the template as an example : `test/test_template.c` or `test/test_server_socket.c`

## Useful commands
- List all running process : `ps -aux | less`
- List process with open ports : `sudo netstat -tulpn`
- Display process with specific port : `sudo lsof -i -P -n | grep <PORT>`
- Kill a process : `sudo kill -9 <PID>`