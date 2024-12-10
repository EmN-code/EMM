
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
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

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    char state[] = "work";  // Device state to send

    while (1) {
        // Send the device state to the server
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            puts("Error: Failed to send UDP packet");
        } else {
            printf("Sent: %s\n", state);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
