#include "xtimer.h"
#include "sock_udp.h"
#include <stdio.h>

static char server_addr_str[] = "47.102.103.1";
#define SERVER_PORT          20001

#define INTERVAL             (5U * US_PER_SEC) // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t state_work = 1;  // 1 for work, 0 for not work

    // Resolve server address
    if (inet_pton(AF_INET, server_addr_str, &remote.addr.ipv4) != 1) {
        puts("Error: unable to resolve server address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create the UDP socket");
        return 1;
    }

    while (1) {
        char *state_msg = state_work ? "work" : "not work";
        printf("Sending state: %s\n", state_msg);
        if (sock_udp_send(&sock, state_msg, sizeof(state_msg), &remote) < 0) {
            puts("Error: unable to send the message");
        }
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}