#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP      "47.102.103.1"
#define UDP_SERVER_PORT    20001
#define INTERVAL           (5 * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);
    remote.port = UDP_SERVER_PORT;

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        printf("Sending device state...\n");
        const char *message = "work or not work";
        sock_udp_send(&sock, message, strlen(message), &remote);

        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Cleanup
    sock_udp_close(&sock);
    return 0;
}