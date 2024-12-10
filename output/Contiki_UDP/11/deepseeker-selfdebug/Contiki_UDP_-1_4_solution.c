
#include "contiki.h"
#include "net/ipv4/uip.h"
#include "net/ipv4/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *udp_conn;
static struct etimer et;

PROCESS(udp_sender_process, "UDP Sender Process");
AUTOSTART_PROCESSES(&udp_sender_process);

void udp_send_state(void) {
    static char buf[] = DEVICE_STATE_MESSAGE;
    uip_ipaddr_t ipaddr;

    uip_ipaddr(&ipaddr, UDP_SERVER_IP);
    uip_udp_packet_send(udp_conn, buf, strlen(buf));
}

PROCESS_THREAD(udp_sender_process, ev, data) {
    PROCESS_BEGIN();

    udp_conn = uip_udp_new(&uip_ip_addr(UDP_SERVER_IP), UIP_HTONS(UDP_SERVER_PORT));
    if(udp_conn == NULL) {
        printf("Failed to create UDP connection\n");
        PROCESS_EXIT();
    }

    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&udp_sender_process, NULL);
    return 0;
}
