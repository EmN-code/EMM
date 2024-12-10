
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

int main(void)
{
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    // Initialize the local endpoint
    local.port = 0;  // Let the system assign a random port

    // Initialize the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    char buffer[256];
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Prepare the message to send
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
            puts("Error: Failed to send UDP message");
        } else {
            printf("Sent: %s\n", buffer);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
