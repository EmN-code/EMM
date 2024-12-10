
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[] = "work";

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state message
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), NULL) < 0) {
            puts("Error: Failed to send UDP message");
        } else {
            printf("Sent: %s\n", state_msg);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
