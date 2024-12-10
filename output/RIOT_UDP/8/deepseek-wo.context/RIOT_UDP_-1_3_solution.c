
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
    snprintf(buf, sizeof(buf), "%s", state);
    sock_udp_send(&sock, buf, strlen(buf), &remote);
}

int main(void) {
    // Initialize the UDP socket
    sock_udp_create(&sock, NULL, NULL, 0);

    // Convert the server IP address from string to binary form
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    // Send device state every 5 seconds
    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL);
        send_state("not work");
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
