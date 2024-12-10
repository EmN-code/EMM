#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/af.h>

#include <msg.h>
#include <stdio.h>
#include <kernel_defines.h>

#define INTERVAL (5U * US_PER_SEC)  // 5 seconds
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

// Function to send device state
void send_state(sock_udp_ep_t *remote) {
    const char *state_message = "work";  // or "not work"
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);
    sock_udp_send(&sock, state_message, strlen(state_message), remote);
    sock_udp_close(&sock);
}

int main(void) {
    // Define remote server address
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Timer struct
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_state(&remote);  // Send device state every 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}