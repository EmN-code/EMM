#include <stdlib.h>
#include <stdio.h>
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki.h"
#include "contiki-net.h"

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct uip_udp_conn *udp_conn;
static char buf[20];
static uip_ipaddr_t server_ipaddr;

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer periodic_timer;
    PROCESS_BEGIN();

    uip_ip6addr(&server_ipaddr, 0x2406,0x3003,0x2073,0x3,0xf9e8,0x83cc,0x6466,0x1); // Translation of IPv4 47.102.103.1 to IPv6
    
    udp_conn = uip_udp_new(&server_ipaddr, UIP_HTONS(20001));
    if (!udp_conn) {
        printf("Failed to establish connection\n");
        PROCESS_EXIT();
    }

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while(1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
      snprintf(buf, sizeof(buf), "work");
      uip_udp_packet_send(udp_conn, buf, strlen(buf));
      etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}