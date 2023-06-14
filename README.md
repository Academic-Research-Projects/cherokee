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
- Install apache bench : `sudo apt install apache2-utils`

## How to use
- Compile the program : `make`
- Run the program : `./cherokee`
- Clean the program : `make clean`
- Clean the program and the executable : `make fclean`
- Re-compile the program : `make re`
- Run the program with valgrind : `valgrind ./cherokee` => Debug mode (memory leaks)
- Run the program with gdb : `gdb ./cherokee` => Debug mode
- Run apache bench : `ab -n 1000 -c 100 http://localhost:8080/`
<!-- - Run criterion tests : `make tests_run` -->
<!-- - Run functional tests : ... -->

## How to test
- Open another terminal
- Execute `curl http://localhost:8080`
- You should see a "Hello, World" in the client terminal
- This is the output expected from the server, in your server terminal :

`GET / HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.74.0
Accept: */*`

## How to test the HTTP POST request, posting a simple text
- Open another terminal
- Execute `curl -X POST -d "The text you want to post" http://localhost:8080`
- You should see the text in your server terminal

## How to test the HTTP POST request, posting a JSON file
- Open another terminal
- Execute `curl -X POST -d "hello antho" http://localhost:8080/file.txt`
- You should see the text in your server terminal

## How to test the HTTP PUTT request, posting a JSON file
- Open another terminal
- Execute `curl -X PUT -d "this file has been modified" http://localhost:8080/file.txt`
- You should see the text in your server terminal