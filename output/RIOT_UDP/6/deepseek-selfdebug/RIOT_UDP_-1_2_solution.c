
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(sock_udp_t *sock, const sock_udp_ep_t *remote)
{
    const char *state = "work";  // or "not work" based on device state
    ssize_t res = sock_udp_send(sock, state, strlen(state), remote);
    if (res < 0) {
        puts("Error sending state");
    } else {
        printf("State sent: %s\n", state);
    }
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_state(&sock, &remote);
        xtimer_usleep(INTERVAL_MS);
    }

    sock_udp_close(&sock);
    return 0;
}
