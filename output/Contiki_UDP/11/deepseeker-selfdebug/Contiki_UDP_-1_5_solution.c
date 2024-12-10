
#include "contiki.h"
#include "net/ipv4/uip.h"
#include "net/ipv4/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define DEVICE_STATE_MESSAGE_LENGTH 4

static struct uip_udp_conn *conn;
static struct etimer et;

void udp_send_state(void) {
    uip_ipaddr_t addr;
    uip_ipaddr(&addr, 47, 102, 103, 1);
    uip_udp_packet_send(conn, DEVICE_STATE_MESSAGE, DEVICE_STATE_MESSAGE_LENGTH);
}

PROCESS(device_state_sender, "Device State Sender");
AUTOSTART_PROCESSES(&device_state_sender);

PROCESS_THREAD(device_state_sender, ev, data) {
    PROCESS_BEGIN();

    // Initialize UDP connection
    conn = uip_udp_new(&addr, UIP_HTONS(UDP_SERVER_PORT));
    if (conn == NULL) {
        printf("Failed to create UDP connection\n");
        PROCESS_EXIT();
    }

    // Set the timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&device_state_sender, NULL);
    return 0;
}
