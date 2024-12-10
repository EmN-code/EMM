
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
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
    static char buf[32];
    sprintf(buf, "%s", DEVICE_STATE_MESSAGE);
    uip_ipaddr_t addr;
    uip_ipaddr(&addr, UDP_SERVER_IP, 0);
    uip_udp_packet_send(conn, buf, strlen(buf));
}

PROCESS(udp_sender_process, "UDP Sender Process");
AUTOSTART_PROCESSES(&udp_sender_process);

PROCESS_THREAD(udp_sender_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the UDP connection
    conn = uip_udp_new(&addr, UIP_HTONS(UDP_SERVER_PORT));
    if(conn == NULL) {
        printf("Error: could not create UDP connection\n");
        PROCESS_EXIT();
    }

    // Set the timer for periodic sending
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
