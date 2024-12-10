#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U) // 5 seconds expressed in microseconds

static const char *state = "work";

int main(void) {
    // UDP setup
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };

    // Convert server IP address from string format to network format
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) < 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    // Create UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }

    while (1) {
        // Send device state as UDP packet
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            printf("Sent state: %s\n", state);
        }

        // Rotate state for the next message (for demonstration purposes)
        state = (strcmp(state, "work") == 0) ? "not work" : "work";

        // Wait for the next interval
        xtimer_usleep(INTERVAL);
    }

    // Close socket (not reachable in this example)
    sock_udp_close(&sock);
    return 0;
}
