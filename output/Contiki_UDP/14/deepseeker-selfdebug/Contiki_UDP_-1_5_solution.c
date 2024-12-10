
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *udp_conn;
static struct etimer et;

void udp_send_state(void) {
    static char buf[32];
    sprintf(buf, "%s", DEVICE_STATE_MESSAGE);
    uip_ipaddr_t ipaddr;
    uip_ipaddr(&ipaddr, UDP_SERVER_IP);
    uip_udp_packet_send(udp_conn, buf, strlen(buf));
}

PROCESS(udp_sender_process, "UDP Sender Process");
AUTOSTART_PROCESSES(&udp_sender_process);

PROCESS_THREAD(udp_sender_process, ev, data) {
    PROCESS_BEGIN();

    udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    udp_bind(udp_conn, UIP_HTONS(UDP_SERVER_PORT));

    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}
