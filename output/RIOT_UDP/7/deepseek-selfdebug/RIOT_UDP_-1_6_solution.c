
#include <stdio.h>
#include <arpa/inet.h>  // Include this header for inet_pton
#include "xtimer.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000U)  // 5 seconds interval

static sock_udp_t sock;
static sock_udp_ep_t remote;

void send_state(const char *state) {
    const char *message = state;
    ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);
    if (res < 0) {
        printf("Failed to send message: %s\n", state);
    } else {
        printf("Sent: %s\n", state);
    }
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    while (1) {
        send_state("work");
        xtimer_sleep(INTERVAL_MS / 1000);
        send_state("not work");
        xtimer_sleep(INTERVAL_MS / 1000);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
