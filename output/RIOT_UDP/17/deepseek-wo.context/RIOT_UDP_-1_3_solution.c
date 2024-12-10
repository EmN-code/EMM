
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;
    char *message = "work";  // or "not work" based on device state
    uint8_t buf[16];  // Buffer to hold the IP address

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Initialize the local endpoint
    local.port = 0;  // Let the system assign a random port
    local.family = AF_INET;

    // Create a UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send the message
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error sending message");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
