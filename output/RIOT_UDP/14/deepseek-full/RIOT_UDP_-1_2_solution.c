
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "sys/socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_SEC 5

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[10];

    // Create UDP socket
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

    // Main loop to send device state every 5 seconds
    while (1) {
        // Prepare the message
        strncpy(buffer, DEVICE_STATE, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        // Send the message
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Sendto failed");
        } else {
            printf("Message sent: %s\n", buffer);
        }

        // Wait for 5 seconds
        sleep(INTERVAL_SEC);
    }

    // Close the socket (this part is unreachable in the current loop)
    close(sockfd);

    return 0;
}
