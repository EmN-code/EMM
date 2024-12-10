
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

void send_state(const char *state) {
    char buf[20];
    strncpy(buf, state, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
        puts("Error sending message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }
    remote.port = SERVER_PORT;

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_state("work");
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
