#include "simple-udp.h"
#include "etimer.h"
#include "contiki-net.h"
#include "contiki.h"

#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer periodic_timer;
    static char device_state[20] = "work";
    
    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    etimer_set(&periodic_timer, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        etimer_reset(&periodic_timer);

        simple_udp_send(&udp_conn, device_state, strlen(device_state));

        printf("Sent device state: %s\n", device_state);
    }

    PROCESS_END();
}