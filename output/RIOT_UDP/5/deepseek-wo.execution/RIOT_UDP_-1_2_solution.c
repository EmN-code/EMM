
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "net/emcute.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_SEC 5

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[256];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        puts("Error creating socket");
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Main loop to send device state every 5 seconds
    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            puts("Error sending data");
        } else {
            puts("Data sent successfully");
        }

        // Wait for 5 seconds before sending the next message
        sleep(INTERVAL_SEC);
    }

    // Close the socket (this part is unreachable in this example)
    close(sock);

    return 0;
}