
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "net/ipv4/addr.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_SEC 5

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[100];

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

    while (1) {
        // Prepare the device state message
        snprintf(buffer, sizeof(buffer), "%s", DEVICE_STATE);

        // Send the device state to the server
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Sendto failed");
            close(sockfd);
            return -1;
        }

        printf("Device state sent: %s\n", buffer);

        // Wait for 5 seconds before sending the next state
        sleep(INTERVAL_SEC);
    }

    // Close the socket (this part is unreachable in the current loop)
    close(sockfd);
    return 0;
}
