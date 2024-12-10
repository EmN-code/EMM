#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET };
    sock_udp_t sock;
    char *state = "work"; // or "not work"
    uint8_t buf[64];

    // Set the remote endpoint
    remote.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send the device state
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            puts("Error sending message");
        } else {
            printf("Sent state: %s\n", state);
        }

        // Wait for the next interval
        xtimer_sleep(SEND_INTERVAL);
    }

    // Close the socket (unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}