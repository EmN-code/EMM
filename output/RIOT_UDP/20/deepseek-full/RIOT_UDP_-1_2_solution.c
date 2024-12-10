
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
    int sock;
    struct sockaddr_in server_addr;
    char buffer[100];

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Failed to create socket");
        return 1;
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send message");
        } else {
            printf("Message sent: %s\n", buffer);
        }

        // Wait for 5 seconds before sending the next message
        sleep(INTERVAL_SEC);
    }

    // Close the socket
    close(sock);

    return 0;
}
