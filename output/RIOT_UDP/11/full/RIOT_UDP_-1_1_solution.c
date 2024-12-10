#include "xtimer.h"
#include "udp.h"
#include "net/gnrc/ipv6.h"
#include "net/sock/udp.h"
#include "stdio.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL (5U * 1000000U)  // 5 seconds interval

void send_device_state(sock_udp_ep_t *remote, const char *state) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return;
    }

    ssize_t res = sock_udp_send(&sock, state, strlen(state), remote);
    if (res < 0) {
        printf("Error sending UDP packet\n");
    } else {
        printf("Sent packet: '%s'\n", state);
    }

    sock_udp_close(&sock);
}

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET,
                             .addr.ipv4 = { {47, 102, 103, 1} },
                             .port = SERVER_PORT };
    char *device_states[2] = { "work", "not work" };
    int idx = 0;

    while (1) {
        send_device_state(&server, device_states[idx]);
        idx = (idx + 1) % 2;  // alternate between "work" and "not work"
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}