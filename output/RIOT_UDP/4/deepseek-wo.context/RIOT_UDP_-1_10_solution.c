
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void send_state(void) {
    const char *state = "work";  // or "not work" based on device state
    ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
    if (res < 0) {
        puts("Error sending state");
    } else {
        printf("State sent: %s\n", state);
    }
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_state();
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}