
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(void) {
    static int state = 0;  // 0 for "not work", 1 for "work"
    const char *state_str = state ? "work" : "not work";
    state = !state;  // Toggle state

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_addr_from_str((struct in_addr *)&remote.addr.ipv4, SERVER_IP);

    sock_udp_t sock;
    sock_udp_ep_t local = { .family = AF_INET };

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    if (sock_udp_send(&sock, state_str, strlen(state_str), &remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        printf("Sent: %s\n", state_str);
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("UDP client starting");

    while (1) {
        send_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
