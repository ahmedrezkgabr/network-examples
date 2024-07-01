// udp_server.cpp
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
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(sockfd);
        return -1;
    }

    while (true)
    {
        socklen_t len = sizeof(cliaddr); // len is value/result
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                         (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        std::cout << "Client : " << buffer << std::endl;

        std::string response = "Message received";
        sendto(sockfd, response.c_str(), response.length(), MSG_CONFIRM,
               (const struct sockaddr *)&cliaddr, len);
        std::cout << "Response sent." << std::endl;
    }

    close(sockfd);
    return 0;
}
