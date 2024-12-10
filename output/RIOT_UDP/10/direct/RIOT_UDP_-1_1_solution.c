#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/af.h>
#include <net/protnum.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL_SEC (5U)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .netif = NETIF, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_IP);
    sock_udp_t sock;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, DEVICE_STATE, strlen(DEVICE_STATE), &server);
        if (res < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Sent device state: %s\n", DEVICE_STATE);
        }
        xtimer_sleep(SEND_INTERVAL_SEC);
    }

    sock_udp_close(&sock);
    return 0;
}