#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server_endpoint = { .family = AF_INET6, .nettype = SOCK_NETTYPE_IPV6, .port = SERVER_PORT };
    inet_pton(server_endpoint.family, SERVER_ADDR, &server_endpoint.addr.ipv6);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    const char *device_state = "work";

    while (1) {
        ssize_t res = sock_udp_send(&sock, device_state, strlen(device_state), &server_endpoint);
        if (res < 0) {
            printf("Failed to send UDP packet: %d\n", res);
        } else {
            printf("Sent device state\n");
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}