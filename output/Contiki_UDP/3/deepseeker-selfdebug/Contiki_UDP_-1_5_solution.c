
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void udp_send_state(void) {
    static char buf[] = DEVICE_STATE;
    uip_ipaddr_t server_ip;
    uip_ip6addr(&server_ip, 0x2001, 0xdb8, 0, 0, 0, 0, 0, 0x1); // Example IPv6 address
    uip_udp_packet_sendto(conn, buf, strlen(buf), &server_ip, UIP_HTONS(SERVER_PORT));
}

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    conn = uip_udp_new(NULL, UIP_HTONS(0));
    if(conn == NULL) {
        printf("Failed to create UDP connection\n");
        PROCESS_EXIT();
    }

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}
