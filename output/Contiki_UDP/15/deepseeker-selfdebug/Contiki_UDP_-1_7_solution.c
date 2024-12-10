
#include <stdio.h>
#include "net/ipv6/udp-socket.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP);

    sock_udp_t sock;
    char buf[] = DEVICE_STATE;

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, buf, sizeof(buf), NULL) < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent device state: %s\n", buf);
        }

        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
