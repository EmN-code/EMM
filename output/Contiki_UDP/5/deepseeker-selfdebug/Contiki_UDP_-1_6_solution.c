
#include <stdio.h>
#include <xtimer.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS 5000000

static void udp_send_state(void) {
    static uint8_t state_msg[] = DEVICE_STATE;
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_aton(UDP_SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);
    sock_udp_send(&sock, state_msg, sizeof(state_msg) - 1, NULL);
    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        udp_send_state();
        xtimer_usleep(INTERVAL_MS);
    }
    return 0;
}
