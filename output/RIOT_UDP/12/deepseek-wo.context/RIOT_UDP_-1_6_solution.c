
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[] = "work";

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;
    char buf[16];

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send the state message
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &remote) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", state_msg);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
