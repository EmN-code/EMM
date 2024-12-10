#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds

static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

void send_device_state(sock_udp_t *sock) {
    const char *state = "work"; // or "not work"
    ssize_t res = sock_udp_send(sock, state, strlen(state), &remote);
    if (res < 0) {
        printf("Error sending message: %d\n", res);
    } else {
        printf("Sent message: %s\n", state);
    }
}

int main(void) {
    sock_udp_t sock;
    int res;

    // Set up the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    // Create the UDP socket
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    while (1) {
        send_device_state(&sock);
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}