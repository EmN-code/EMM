
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Include this header for inet_pton
#include "xtimer.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DELAY_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(sock_udp_t *sock, const char *state) {
    sock_udp_ep_t remote = { .family = AF_INET };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    ssize_t res = sock_udp_send(sock, state, strlen(state), &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent state: %s\n", state);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;  // Use any available local port

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_state(&sock, "work");
        xtimer_usleep(DELAY_MS);
        send_state(&sock, "not work");
        xtimer_usleep(DELAY_MS);
    }

    sock_udp_close(&sock);
    return 0;
}
