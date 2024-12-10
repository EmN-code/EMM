
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL 5000000  // 5 seconds in microseconds

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[100];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    while (1) {
        // Send message to server
        strcpy(buffer, MESSAGE);
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Send failed");
            return -1;
        }

        // Wait for 5 seconds
        usleep(INTERVAL);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
