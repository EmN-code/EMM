
#include <xtimer.h>
#include <net/ipv6/simple-udp.h>
#include <net/ipv6/udp-socket.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    udp_socket_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4.u32);

    if (udp_socket(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        const char *state = DEVICE_STATE;
        ssize_t res = udp_send(&sock, state, strlen(state), &remote);
        if (res < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent device state: %s\n", state);
        }

        xtimer_usleep(INTERVAL);
    }

    udp_close(&sock);
    return 0;
}
