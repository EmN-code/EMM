#include <ztimer.h>
#include "net/sock/udp.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT    20001
#define SEND_INTERVAL  (5U * ZTIMER_SEC)

static sock_udp_ep_t server_ep;

void send_device_state(sock_udp_t *sock) {
    const char *state = "work";  // Assuming the device state is always "work"
    ssize_t res = sock_udp_send(sock, state, strlen(state), &server_ep);
    if (res < 0) {
        printf("Error sending message: %d\n", res);
    }
    else {
        printf("State sent: %s\n", state);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = { .port = 0 };

    int res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    // Set up the server endpoint
    server_ep.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_ADDRESS, &server_ep.addr.ipv4) == 0) {
        printf("Error setting server IP\n");
        return 1;
    }

    while (1) {
        send_device_state(&sock);
        ztimer_sleep(ZTIMER_MSEC, SEND_INTERVAL);
    }

    return 0;
}