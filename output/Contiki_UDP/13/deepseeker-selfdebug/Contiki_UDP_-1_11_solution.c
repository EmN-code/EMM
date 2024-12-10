
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work" // or "not work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void udp_send_state(void) {
    static uint8_t buf[16];
    int len = snprintf((char *)buf, sizeof(buf), "%s", DEVICE_STATE);

    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, 0, 0, 0, 0, 0, 0, 0, 0); // Initialize IP address
    uip_ip6addr(&server_ip, 0x2001, 0xdb8, 0, 0, 0, 0, 0, 1); // Example IPv6 address

    uip_udp_conn_t *conn = udp_new(&server_ip, UIP_HTONS(UDP_SERVER_PORT), NULL);
    if (conn != NULL) {
        udp_send(conn, buf, len);
        udp_remove(conn);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        udp_send_state();
        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}
