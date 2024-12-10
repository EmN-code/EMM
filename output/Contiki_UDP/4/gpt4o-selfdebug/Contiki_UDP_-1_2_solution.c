#include "contiki-net.h"  
#include "contiki-default-conf.h"  
#include "contiki-lib.h"  
#include "contiki-conf.h" 
#include "net/routing/routing.h"

#include <stdio.h>
#include <stdbool.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_IP "47.102.103.1"
#define SEND_INTERVAL 5 * CLOCK_SECOND

static struct uip_udp_conn *client_conn;
static char *device_state = "work";

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer periodic_timer;

    PROCESS_BEGIN();

    printf("Starting UDP client\n");

    client_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
    if (client_conn == NULL) {
        printf("No UDP connection available, exiting process\n");
        PROCESS_EXIT();
    }
    udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));

    printf("Connecting to UDP server at %s:%d\n", UDP_SERVER_IP, UDP_SERVER_PORT);

    etimer_set(&periodic_timer, SEND_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        printf("Sending device state: %s\n", device_state);

        uip_ipaddr_t server_ipaddr;
        uiplib_ipaddrconv(UDP_SERVER_IP, &server_ipaddr);  // Convert string IP address to uIP address format

        uip_udp_packet_sendto(client_conn, device_state, strlen(device_state), 
                              &server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}
