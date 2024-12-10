
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345

static struct uip_udp_conn *udp_conn;
static struct etimer periodic_timer;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void udp_send_state(const char *state) {
    uip_ipaddr_t server_ip;
    uip_ip6addr(&server_ip, 0x2f, 0x66, 0x67, 0x67, 0, 0, 0, 0); // Convert IP to IPv6 format
    uip_udp_packet_send(udp_conn, state, strlen(state));
}

PROCESS_THREAD(udp_client_process, ev, data) {
    static char state[] = "work";

    PROCESS_BEGIN();

    udp_conn = uip_udp_new(&server_ip, UIP_HTONS(UDP_SERVER_PORT));
    if(udp_conn == NULL) {
        printf("Error: could not create UDP connection\n");
        PROCESS_EXIT();
    }
    uip_udp_bind(udp_conn, UIP_HTONS(UDP_CLIENT_PORT));

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        udp_send_state(state);
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}
