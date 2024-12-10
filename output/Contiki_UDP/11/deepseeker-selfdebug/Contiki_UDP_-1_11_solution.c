
#include <xtimer.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/ipv6/udp-socket.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS 5000000

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4.u32);

    sock_udp_t sock;
    char buf[] = DEVICE_STATE;

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, buf, sizeof(buf), &remote) < 0) {
            puts("Error sending UDP packet");
        } else {
            puts("UDP packet sent");
        }
        xtimer_usleep(INTERVAL_MS);
    }

    sock_udp_close(&sock);
    return 0;
}
