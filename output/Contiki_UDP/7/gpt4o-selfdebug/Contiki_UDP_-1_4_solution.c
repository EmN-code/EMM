
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/psock.h"

#include "contiki.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct uip_udp_conn *client_conn;
static char *device_state = "work or not work";

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer periodic_timer;

    PROCESS_BEGIN();
    
    printf("UDP client process started\n");

    etimer_set(&periodic_timer, SEND_INTERVAL);

    client_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
    if (!client_conn) {
        printf("Failed to open UDP connection\n");
        PROCESS_EXIT();
    }
    
    uiplib_ipaddrconv(UDP_SERVER_IP, &client_conn->ripaddr);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&periodic_timer)) {
            etimer_reset(&periodic_timer);

            printf("Sending device state: %s\n", device_state);
            uip_udp_packet_send(client_conn, device_state, strlen(device_state));
        }
    }

    PROCESS_END();
}