#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_PORT (20001)
#define LOCAL_PORT (8808)
#define SERVER_ADDR "47.102.103.1"
#define INTERVAL (5000000U)  // Interval of 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = LOCAL_PORT;

    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    const char *work_state = "work";
    const char *idle_state = "not work";

    while (1) {
        if (sock_udp_send(&sock, work_state, strlen(work_state), &remote) < 0) {
            puts("Error: unable to send state");
        } else {
            printf("State sent: %s\n", work_state);
        }

        xtimer_usleep(INTERVAL);

        if (sock_udp_send(&sock, idle_state, strlen(idle_state), &remote) < 0) {
            puts("Error: unable to send state");
        } else {
            printf("State sent: %s\n", idle_state);
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}