
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"

static struct uip_udp_conn *udp_conn;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_udp_packet(void) {
    static char buf[] = DEVICE_STATE;
    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, 0x2f, 0x66, 0x67, 0x67, 0, 0, 0, 0);  // Convert IP to uip_ipaddr_t
    uip_udp_packet_send(udp_conn, buf, sizeof(buf) - 1);
}

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    // Initialize UDP connection
    udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, 0x2f, 0x66, 0x67, 0x67, 0, 0, 0, 0);  // Convert IP to uip_ipaddr_t
    udp_bind(udp_conn, UIP_HTONS(UDP_SERVER_PORT));
    udp_connect(udp_conn, &ipaddr, UIP_HTONS(UDP_SERVER_PORT));

    // Set timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        send_udp_packet();

        // Reset timer for next packet
        etimer_reset(&et);
    }

    PROCESS_END();
}
