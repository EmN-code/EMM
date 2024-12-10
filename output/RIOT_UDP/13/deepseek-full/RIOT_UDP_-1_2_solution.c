
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL 5000000  // 5 seconds in microseconds

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[100];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("Failed to create socket");
        return 1;
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(sock);
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
        } else {
            printf("Message sent: %s\n", buffer);
        }

        // Wait for 5 seconds
        usleep(SEND_INTERVAL);
    }

    // Close the socket
    close(sock);
    return 0;
}
