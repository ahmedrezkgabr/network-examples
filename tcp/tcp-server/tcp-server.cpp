// tcp_server.cpp
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, new_socket;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    memset(&address, 0, sizeof(address));

    // Filling server information
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return -1;
    }

    while (true)
    {
        std::cout << "Waiting for a connection..." << std::endl;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            std::cerr << "Accept failed" << std::endl;
            close(server_fd);
            return -1;
        }

        std::cout << "Connection established" << std::endl;

        int n;
        while ((n = read(new_socket, buffer, BUFFER_SIZE)) > 0)
        {
            buffer[n] = '\0';
            std::cout << "Client : " << buffer << std::endl;

            std::string response = "Message received";
            send(new_socket, response.c_str(), response.length(), 0);
            std::cout << "Response sent." << std::endl;
        }

        close(new_socket);
        std::cout << "Connection closed" << std::endl;
    }

    close(server_fd);
    return 0;
}
