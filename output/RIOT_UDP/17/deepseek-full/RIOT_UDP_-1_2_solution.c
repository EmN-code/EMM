
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "net/sock.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "sys/socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL 5000000  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buffer[128];

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state message
        strcpy(buffer, DEVICE_STATE_MSG);
        if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
            puts("Error: Failed to send message");
        } else {
            printf("Message sent: %s\n", buffer);
        }

        // Wait for 5 seconds before sending the next message
        usleep(SEND_INTERVAL);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
