
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "xtimer.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_US (5 * 1000 * 1000)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void send_state(const char *state) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Device state: %s", state);
    ssize_t res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        printf("Failed to send UDP message\n");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;  // Let the system assign a random port

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    // Set up the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return 1;
    }

    // Send device state every 5 seconds
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        send_state("work");
        xtimer_usleep_until(&last_wakeup, INTERVAL_US);
        send_state("not work");
        xtimer_usleep_until(&last_wakeup, INTERVAL_US);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
