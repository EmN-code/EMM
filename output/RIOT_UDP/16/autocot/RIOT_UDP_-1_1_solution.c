#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "net/af.h"
#include "net/protnum.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    printf("Starting UDP client\n");

    sock_udp_ep_t server = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_IP);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        const char *state = "work"; // change to "not work" as per your logic

        ssize_t res = sock_udp_send(&sock, state, strlen(state), &server);

        if (res < 0) {
            printf("Error sending data: %d\n", res);
        } else {
            printf("Sent: %s\n", state);
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}