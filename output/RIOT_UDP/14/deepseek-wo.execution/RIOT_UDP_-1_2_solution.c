
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_addr_from_str(remote.addr.ipv4, SERVER_IP);

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    char buf[64];
    while (1) {
        snprintf(buf, sizeof(buf), "Device state: %s", DEVICE_STATE);
        if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
            puts("Error sending UDP packet");
        } else {
            puts("UDP packet sent");
        }
        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
