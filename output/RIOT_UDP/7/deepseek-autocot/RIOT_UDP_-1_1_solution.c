
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;

void send_state(const char *state) {
    char buf[32];
    int res;

    snprintf(buf, sizeof(buf), "Device state: %s", state);
    res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    int res;

    // Initialize the UDP socket
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error parsing server IP address");
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL_MS);
        send_state("not work");
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}