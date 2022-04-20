// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");

        exit(EXIT_FAILURE);
    }

    printf("TCP socket descriptor: %d\n", server_fd);

    // Setting some options (SO_REUSEADDR | SO_REUSEPORT) at socket level (SOL_SOCKET)
    // In this case, prevents errors such as address already in use
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;           // Socket family
    address.sin_addr.s_addr = INADDR_ANY;   // Bind the server to the localhost IP (INADDR_ANY)
    address.sin_port = htons(PORT);         // Socket port: 8080; htons() function makes sure that numbers are stored in
                                            // memory in network byte order, which is with the most significant byte first.
                                            // i.e. big-endian

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // accept() is a blocking call (i.e. blocks until there is a connection request)
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Client socket descriptor: %d\n", new_socket);

    valread = read(new_socket, buffer, 1024);       // read and send are system calls
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    
    return 0;
}
