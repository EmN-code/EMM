
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[10];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("Failed to create socket");
        return 1;
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sock);
        return 1;
    }

    // Main loop to send device state every 5 seconds
    while (1) {
        // Prepare the message
        strncpy(buffer, DEVICE_STATE, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        // Send the message to the server
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send message");
        } else {
            printf("Message sent: %s\n", buffer);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the socket (this part is unreachable in this example due to the infinite loop)
    close(sock);
    return 0;
}
