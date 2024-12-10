
#include <stdio.h>
#include <xtimer.h>
#include <net/ipv6/uip.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/netstack.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS 5000000

static void udp_send_state(void) {
    static uint8_t payload[] = DEVICE_STATE;
    static uint8_t buffer[UIP_BUFSIZE];
    uip_ipaddr_t server_ip;
    uip_ip6addr(&server_ip, 0x2001, 0xdb8, 0, 0, 0, 0, 0, 0x1);  // Example IPv6 address

    uip_udp_conn_t *conn = uip_udp_new(&server_ip, UIP_HTONS(UDP_SERVER_PORT));
    if (conn == NULL) {
        puts("Error: could not create UDP connection");
        return;
    }

    uip_udp_packet_send(conn, payload, sizeof(payload) - 1);
    uip_udp_remove(conn);
}

int main(void) {
    puts("Starting UDP state sender...");

    while (1) {
        udp_send_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
