// udp_client.cpp
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    while (true)
    {
        std::string message;
        std::cout << "Enter message to send (type 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit")
        {
            break;
        }

        sendto(sockfd, message.c_str(), message.size(), MSG_CONFIRM,
               (const struct sockaddr *)&servaddr, sizeof(servaddr));
        std::cout << "Message sent." << std::endl;

        socklen_t len = sizeof(servaddr);
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                         (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        std::cout << "Server : " << buffer << std::endl;
    }

    close(sockfd);
    return 0;
}
