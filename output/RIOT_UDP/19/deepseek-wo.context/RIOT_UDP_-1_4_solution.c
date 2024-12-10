
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "xtimer.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static const sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT,
};

static void send_state(sock_udp_t *sock) {
    static const char *state = "work";  // or "not work" based on device state
    ssize_t res;

    res = sock_udp_send(sock, state, strlen(state), &server);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent state: %s\n", state);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345;  // Local port to bind to

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) <= 0) {
        puts("Invalid address/ Address not supported");
        return 1;
    }

    while (1) {
        send_state(&sock);
        xtimer_usleep(INTERVAL_MS);
    }

    sock_udp_close(&sock);
    return 0;
}
