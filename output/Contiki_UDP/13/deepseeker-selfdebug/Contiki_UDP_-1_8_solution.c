
#include <stdio.h>
#include <xtimer.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT, .addr = { .ipv4 = { 47, 102, 103, 1 } } };
    sock_udp_t sock;
    char buf[] = DEVICE_STATE;

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, buf, sizeof(buf), &remote) < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent: %s\n", buf);
        }

        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
