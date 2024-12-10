
#include "contiki.h"
#include "net/ipv4/uip.h"
#include "net/ipv4/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work" // or "not work"

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void udp_send_state(void) {
    static char buf[] = DEVICE_STATE;
    uip_ipaddr_t addr;
    uip_ipaddr(&addr, 47, 102, 103, 1);
    uip_udp_packet_send(conn, buf, sizeof(buf) - 1);
}

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize UDP connection
    conn = uip_udp_new(&uip_ip_addr(UDP_SERVER_IP), UIP_HTONS(UDP_SERVER_PORT));
    if(conn == NULL) {
        printf("Failed to create UDP connection\n");
        PROCESS_EXIT();
    }

    // Set up the periodic timer
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}
