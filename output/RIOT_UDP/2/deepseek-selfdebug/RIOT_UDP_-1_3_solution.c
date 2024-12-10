
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Include this header for inet_pton
#include "xtimer.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

void send_state(const char *state) {
    char buf[256];
    int res;

    res = snprintf(buf, sizeof(buf), "Device state: %s", state);
    if (res < 0 || res >= (int)sizeof(buf)) {
        puts("Message too long");
        return;
    }

    if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
        puts("Error sending message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    int res;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL_MS);
        send_state("not work");
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
