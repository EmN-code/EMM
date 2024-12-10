#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    const char *device_state = "work";  // Example device state

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    if (sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) == 0) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    while (1) {
        sock_udp_send(&sock, device_state, strlen(device_state), &remote);
        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}