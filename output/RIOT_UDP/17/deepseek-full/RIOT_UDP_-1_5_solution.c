
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <stdbool.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DELAY_SECONDS 5

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void send_device_state(void) {
    static const char *state = "work"; // or "not work" based on device state
    ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
    if (res < 0) {
        printf("Failed to send data\n");
    } else {
        printf("Sent device state: %s\n", state);
    }
}

int main(void) {
    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    while (true) {
        send_device_state();
        ztimer_sleep(ZTIMER_SEC, DELAY_SECONDS);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);
    return 0;
}
