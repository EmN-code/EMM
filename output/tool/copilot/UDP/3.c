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
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    // Resolve the server address
    res = ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_ADDR);
    if (res == 0) {
        printf("Error resolving server address\n");
        return 1;
    }

    while (1) {
        send_device_state(&sock, "work");
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
        send_device_state(&sock, "not work");
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }

    return 0;
}