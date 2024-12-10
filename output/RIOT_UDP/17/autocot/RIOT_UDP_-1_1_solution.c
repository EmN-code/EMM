#include <string.h>
#include <stdio.h>
#include <net/gnrc/udp.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_US (5 * US_PER_SEC)

static void _send_state(gnrc_udp_t *sock) {
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    ipv6_addr_from_str((ipv6_addr_t*)&remote.addr.ipv6, SERVER_IP);

    int res = sock_udp_send(sock, DEVICE_STATE, strlen(DEVICE_STATE), &remote);
    if (res < 0) {
        printf("Error sending data\n");
    } else {
        printf("Sent: %s\n", DEVICE_STATE);
    }
}

int main(void) {
    gnrc_udp_t udp_sock;
    memset(&udp_sock, 0, sizeof(udp_sock));

    if (gnrc_udp_create(&udp_sock, INET6_ADDRSTRLEN, INET6_ADDRSTRLEN) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        _send_state(&udp_sock);
        xtimer_usleep(INTERVAL_US);
    }

    return 0;
}