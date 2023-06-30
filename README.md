# Groupe de blanch_b 1003936
login: blanch_b loizag_g utzman_f baudet_c

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

## How to test
- Open another terminal
- Execute `curl http://localhost:8080`
- You should see a "Hello, World" in the client terminal
- This is the output expected from the server, in your server terminal :

`GET / HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.74.0
Accept: */*`

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