#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    int res = sock_udp_create(&sock, NULL, NULL, 0);

    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    char *state = "work";

    while (1) {
        ssize_t sent = sock_udp_send(&sock, state, strlen(state), &remote);
        if (sent <= 0) {
            printf("Failed to send UDP packet\n");
        } else {
            printf("Successfully sent UDP packet: %s\n", state);
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}