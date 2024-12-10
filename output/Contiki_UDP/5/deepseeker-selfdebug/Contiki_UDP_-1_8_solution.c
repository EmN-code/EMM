
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_SECONDS 5

static void udp_send_state(void) {
    static uint8_t buf[16];
    int len = snprintf((char *)buf, sizeof(buf), "%s", DEVICE_STATE);

    uip_ipaddr_t server_ip;
    uip_ip6addr(&server_ip, 0, 0, 0, 0, 0, 0, 0, 0);  // IPv4-mapped IPv6 address
    uip_udp_packet_sendto(NULL, buf, len, &server_ip, htons(UDP_SERVER_PORT));
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        udp_send_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_SECONDS * 1000 * 1000);
    }

    return 0;
}
