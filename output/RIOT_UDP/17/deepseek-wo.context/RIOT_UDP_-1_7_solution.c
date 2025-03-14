
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buf[64];
    int res;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    // Convert the server IP address from string to binary form
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the message to send
        snprintf(buf, sizeof(buf), "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
            puts("Error: Failed to send UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
