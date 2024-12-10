
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state[] = "work";  // or "not work"

static void send_state(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    uint8_t addr_bytes[sizeof(struct in_addr)];
    if (inet_pton(AF_INET, SERVER_IP, addr_bytes) <= 0) {
        puts("Error converting IP address");
        return;
    }
    memcpy(&remote.addr.ipv4.u8, addr_bytes, sizeof(addr_bytes));

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        printf("Sent state: %s\n", state);
    }

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
