
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

void send_state(const char *state) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Device state: %s", state);
    ssize_t res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345;  // Local port for the device

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set up the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL);
        send_state("not work");
        xtimer_usleep(INTERVAL);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
