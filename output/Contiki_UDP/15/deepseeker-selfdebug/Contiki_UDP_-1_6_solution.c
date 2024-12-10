
#include <stdio.h>
#include "net/ipv6/udp-socket.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL 5  // in seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4.u32);

    sock_udp_t sock;
    sock_udp_ep_t local = { .family = AF_INET };

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    char buf[] = DEVICE_STATE;
    while (1) {
        if (sock_udp_send(&sock, buf, sizeof(buf), NULL) < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent: %s\n", buf);
        }
        xtimer_sleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
