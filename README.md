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
- Execute `curl http://localhost:8080/zoubisou.json`
- You should see a "Hello, World" in the client terminal
- This is the output expected from the server, in your server terminal :

`GET / HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.74.0
Accept: */*`

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

## WORK IN PROGRESS
- Code compiles
- J'ai essayé de faire marcher la fonction createError404(response), mais je me tape un segfault. file_fd donne autre valeur que -1 quand le fichier n'existe pas, je ne comprends pas pourquoi.
- Comme cela ne marchait pas, j'ai essayé de faire marcher la fonction createSuccess200(response, content_type). Toujours de segfault, le code ne rentre pas dans la condition if.
- Pour tester la fonction createError404(response), j'exécute depuis le client un simple "curl http://localhost:8080/hola.html (ce fichier n'existe pas donc la valeur de fd devrait être -1, or ça donne toujours un valeur positif)".
- Pour tester la fonction createSuccess200(response, content_type)  j'exécute depuis le client un simple "curl http://localhost:8080/zoubisou.json, ce fichier existe et se trouve dans le repertoire test_files
- REMARQUE : à chaque fois qu'on veut tester une fonction, il faut décommenter les fichiers nécessaires sur le Makefile.
- REMARQUE 1 : moi j'ai bougé entre multiplex.c + http_get.c + les fichiers avec les fonctions correspondantes à tester (*.h et *.c respectivement)

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