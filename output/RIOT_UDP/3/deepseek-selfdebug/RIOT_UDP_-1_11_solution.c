
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static const char *states[] = {"work", "not work"};
static int current_state = 0;

static void send_state(sock_udp_t *sock, const sock_udp_ep_t *remote) {
    char buf[16];
    int res = snprintf(buf, sizeof(buf), "%s", states[current_state]);
    if (res < 0) {
        puts("Error formatting message");
        return;
    }

    ssize_t sent = sock_udp_send(sock, buf, strlen(buf), remote);
    if (sent < 0) {
        puts("Error sending message");
    } else {
        printf("Sent: %s\n", buf);
    }

    current_state = !current_state;  // Toggle state
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_state(&sock, &remote);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
