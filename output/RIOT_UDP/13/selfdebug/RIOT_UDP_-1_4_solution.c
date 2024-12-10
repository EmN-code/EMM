#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    /* Initialize remote server endpoint */
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    int res = inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    if (res <= 0) {
        if (res == 0) {
            puts("Invalid IP address format.");
        } else {
            puts("inet_pton failed.");
        }
        return -1;
    }
    
    /* Create UDP socket */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    char *state = "work or not work";
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        if (sock_udp_send(&sock, state, strlen(state), &server) < 0) {
            puts("Error sending UDP packet");
        } else {
            puts("Sent state to server");
        }

        /* Sleep for the specified interval */
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}