
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static const char *states[] = {"work", "not work"};

static void send_state(sock_udp_t *sock, const sock_udp_ep_t *remote, int state)
{
    char buf[16];
    int len = snprintf(buf, sizeof(buf), "%s", states[state]);
    ssize_t res = sock_udp_send(sock, buf, len, remote);
    if (res < 0) {
        puts("Error sending UDP packet");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    local.port = 12345;  // Local port to bind to
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    int state = 0;
    while (1) {
        send_state(&sock, &remote, state);
        state = !state;  // Toggle state
        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
