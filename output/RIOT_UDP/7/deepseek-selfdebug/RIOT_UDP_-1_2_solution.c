
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state[] = "work";  // Device state to send

static void send_state(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buf[64];
    ssize_t res;

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return;
    }

    // Initialize UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    // Send state message
    res = sock_udp_send(&sock, state, strlen(state), &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent state: %s\n", state);
    }

    // Close UDP socket
    sock_udp_close(&sock);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
