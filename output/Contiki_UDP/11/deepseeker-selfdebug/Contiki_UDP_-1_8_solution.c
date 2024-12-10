
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

static struct uip_udp_conn *conn;
static struct etimer et;

void udp_send_state(void) {
    static char buf[] = DEVICE_STATE_MESSAGE;
    uip_ipaddr_t addr;
    uip_ipaddr(&addr, 47, 102, 103, 1);
    uip_udp_packet_send(conn, buf, strlen(buf));
}

PROCESS(udp_sender_process, "UDP Sender Process");
AUTOSTART_PROCESSES(&udp_sender_process);

PROCESS_THREAD(udp_sender_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the UDP connection
    conn = uip_udp_new(&addr, UIP_HTONS(UDP_SERVER_PORT));
    if (conn == NULL) {
        printf("Failed to create UDP connection\n");
        PROCESS_EXIT();
    }

    // Set the timer for sending the state every 5 seconds
    etimer_set(&et, SEND_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}
