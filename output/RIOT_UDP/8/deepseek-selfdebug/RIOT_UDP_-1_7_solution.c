
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <netinet/in.h>  // Include this for inet_pton
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static const char *states[] = {"work", "not work"};

static void send_state(sock_udp_t *sock, const sock_udp_ep_t *remote) {
    static int state_index = 0;
    const char *state = states[state_index];
    ssize_t res = sock_udp_send(sock, state, strlen(state), remote);
    if (res < 0) {
        puts("Error sending UDP packet");
    } else {
        printf("Sent state: %s\n", state);
    }
    state_index = 1 - state_index;  // Toggle between 0 and 1
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Initialize remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_state(&sock, &remote);
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the socket (though the loop will never exit)
    sock_udp_close(&sock);
    return 0;
}
