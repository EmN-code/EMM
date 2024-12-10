#include "uip.h"
#include "simple-udp.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 10000
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer periodic_timer;
    static char device_state[15];

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    etimer_set(&periodic_timer, SEND_INTERVAL);
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        // Determining the device state
        // Here, the device status is simulated as "work" or "not work".
        snprintf(device_state, sizeof(device_state), "work"); // Example state

        uip_ipaddr_t server_ipaddr;
        uip_ipaddr(&server_ipaddr, 47,102,103,1);
        simple_udp_sendto(&udp_conn, device_state, sizeof(device_state), &server_ipaddr);

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}
