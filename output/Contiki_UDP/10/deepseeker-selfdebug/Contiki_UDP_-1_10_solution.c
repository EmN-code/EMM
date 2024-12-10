
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void udp_send_state(void) {
    static char buf[] = DEVICE_STATE;
    uip_ipaddr_t addr;
    uip_ipaddr(&addr, UDP_SERVER_IP);
    uip_udp_packet_send(conn, buf, strlen(buf));
}

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    conn = uip_udp_new(&addr, UIP_HTONS(UDP_SERVER_PORT));
    if(conn == NULL) {
        printf("Error: could not create UDP connection\n");
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

int main(void) {
    process_start(&udp_client_process, NULL);
    return 0;
}
