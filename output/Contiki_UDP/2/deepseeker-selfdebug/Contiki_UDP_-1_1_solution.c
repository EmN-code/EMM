
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(device_state_sender, "Device State Sender");
AUTOSTART_PROCESSES(&device_state_sender);

static void udp_send_state(void) {
    static char buffer[32];
    strncpy(buffer, DEVICE_STATE_MESSAGE, sizeof(buffer));
    simple_udp_sendto(&udp_conn, buffer, strlen(buffer), (uip_ipaddr_t *)UDP_SERVER_IP, UDP_SERVER_PORT);
}

PROCESS_THREAD(device_state_sender, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    etimer_set(&et, SEND_INTERVAL);

    while(1) {
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
