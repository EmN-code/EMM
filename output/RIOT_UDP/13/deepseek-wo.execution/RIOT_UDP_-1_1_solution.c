
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

void send_state(const char *state)
{
    char buf[32];
    int res;

    snprintf(buf, sizeof(buf), "Device state: %s", state);
    res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the UDP socket
    sock_udp_ep_set_ipaddr(&remote, IPV4_ADDR(47, 102, 103, 1));
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Send "work" state
        send_state("work");

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);

        // Send "not work" state
        send_state("not work");

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
