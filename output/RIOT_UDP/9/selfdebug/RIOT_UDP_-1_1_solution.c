#include <stdio.h>
#include <string.h>
#include "network.h"
#include "net/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U)  // 5 seconds

void send_device_state(sock_udp_ep_t *remote) {
    const char *state = "work";  // Define your device state here
    sock_udp_t sock;

    if (sock_udp_create(&sock, NULL, NULL, 0) != 0) {
        printf("Error: Unable to create UDP socket\n");
        return;
    }

    if (sock_udp_send(&sock, state, strlen(state), remote) < 0) {
        printf("Error: Unable to send UDP packet\n");
    } else {
        printf("Sent UDP packet with state: %s\n", state);
    }

    sock_udp_close(&sock);
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    if (inet_pton(AF_INET6, SERVER_ADDR, &remote.addr.ipv6) != 1) {
        printf("Error: Unable to parse server address\n");
        return 1;
    }

    while (1) {
        send_device_state(&remote);
        xtimer_usleep(SEND_INTERVAL);
    }
    return 0;
}