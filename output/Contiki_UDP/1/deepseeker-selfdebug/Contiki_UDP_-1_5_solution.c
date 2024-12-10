
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345

static struct uip_udp_conn *conn;
static struct etimer et;

void udp_send_state(const char *state) {
    uip_ipaddr_t server_ip;
    uiplib_ipaddrconv(UDP_SERVER_IP, server_ip);

    uip_ipaddr(server_ip, 47, 102, 103, 1);

    uip_udp_packet_sendto(conn, state, strlen(state), &server_ip, UIP_HTONS(UDP_SERVER_PORT));
}

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    static char state[] = "work";

    PROCESS_BEGIN();

    conn = uip_udp_new(&uip_broadcast_ipaddr, UIP_HTONS(UDP_CLIENT_PORT));
    if(conn == NULL) {
        printf("Error: could not create UDP connection\n");
        PROCESS_EXIT();
    }

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        udp_send_state(state);

        etimer_reset(&et);
    }

    PROCESS_END();
}
