
#include <stdio.h>
#include <xtimer.h>
#include <net/ipv6/udp-socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL 5  // in seconds

int main(void)
{
    // Initialize UDP socket
    sock_udp_ep_t local = { .port = 0, .family = AF_INET };
    sock_udp_ep_t remote = { .addr = SERVER_IP, .port = SERVER_PORT, .family = AF_INET };
    sock_udp_t *sock;
    char buf[] = DEVICE_STATE;

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(sock, buf, sizeof(buf), &remote) < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent: %s\n", buf);
        }

        xtimer_sleep(INTERVAL);
    }

    sock_udp_close(sock);
    return 0;
}
