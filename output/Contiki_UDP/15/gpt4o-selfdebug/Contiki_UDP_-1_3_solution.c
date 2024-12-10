#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include <stdio.h>
#include "contiki.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"

static struct uip_udp_conn *udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer timer;
    static char *device_state;
    uip_ipaddr_t server_ipaddr;

    PROCESS_BEGIN();

    // Create a new UDP connection to the server
    uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);
    udp_conn = udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
    if(!udp_conn) {
        printf("Failed to create a new UDP connection\n");
    }

    // Connect UDP connection
    udp_bind(udp_conn, UIP_HTONS(0));

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Toggle device state between 'work' and 'not work'
        device_state = (device_state == DEVICE_STATE_WORK) ? DEVICE_STATE_NOT_WORK : DEVICE_STATE_WORK;

        // Send UDP packet with device state
        uip_udp_packet_send(udp_conn, device_state, strlen(device_state));
        printf("Sent device state: %s\n", device_state);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}