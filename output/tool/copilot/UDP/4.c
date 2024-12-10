#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "ztimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * US_PER_SEC) // 5 seconds

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static void send_device_state(sock_udp_t *sock, const char *state) {
    ssize_t res = sock_udp_send(sock, state, strlen(state), &server);
    if (res < 0) {
        printf("Error sending message: %d\n", res);
    } else {
        printf("Sent: %s\n", state);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;

    // Resolve server address
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    // Main loop
    while (1) {
        send_device_state(&sock, "work");
        xtimer_sleep(5);
        send_device_state(&sock, "not work");
        xtimer_sleep(5);
    }

    return 0;
}