#!/bin/bash

echo "Starting demo..."

# curl GET
echo "curl GET index.html"
curl http://localhost:8080/index.html
echo -e "\n\n------------------ \n"
echo -e "curl GET back to the future JPG \n"
# Download and output dev/null
curl http://localhost:8080/back-to-the-future.jpg --output /dev/null

# curl HEAD
echo -e "\n\n------------------"
echo "curl HEAD file.txt"
curl -I -X HEAD http://localhost:8080/file.txt

# curl POST
echo -e "\n\n------------------"
echo "curl POST index-demo.html"
curl -X POST -d "Hello World" http://localhost:8080/index-demo.html
echo -e "\n------------------\n"
echo -e "curl GET index-demo.html"
curl http://localhost:8080/index-demo.html

# curl PUT

echo -e "\n\n------------------"
echo "curl PUT index-demo.html"
curl -X PUT -d "Hello World 2" http://localhost:8080/index-demo.html
echo -e "\n------------------\ncurl GET index-demo.html"
curl http://localhost:8080/index-demo.html

# curl DELETE
echo -e "\n\n------------------\n"
echo "curl DELETE index-demo.html"
curl -X DELETE http://localhost:8080/index-demo.html

# Input validation to start siege
echo -e "\n\n------------------"
echo "Start siege? (y/n)"
read siege
if [ $siege == "y" ]
then
  echo "Starting siege..."
  siege -c 255 -t10s '127.0.0.1:8080/index.html'
else
  echo "Exiting..."
fi