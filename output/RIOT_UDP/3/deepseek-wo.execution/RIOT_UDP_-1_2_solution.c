
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/sock.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL 5000000  // 5 seconds in microseconds

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[100];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        // Prepare the message
        strcpy(buffer, DEVICE_STATE);

        // Send the message to the server
        if (sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
            close(sockfd);
            return -1;
        }

        printf("Message sent: %s\n", buffer);

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
