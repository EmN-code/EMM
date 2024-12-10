#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include <string.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define UDP_IP_ADDRESS "47.102.103.1"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer periodic_timer;
    static uip_ipaddr_t server_ipaddr;
    const char *state_message = "work";  // You can set this dynamically based on your device logic

    PROCESS_BEGIN();

    // Convert the string IP address to a uip_ipaddr_t
    // Here, hardcoding the IPv6 equivalent address
    uip_ip6addr(&server_ipaddr, 0x2001, 0, 0, 0, 0, 0, 0, 0x1);

    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, &server_ipaddr, UDP_SERVER_PORT, NULL);

    etimer_set(&periodic_timer, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        printf("Sending message to server\n");
        simple_udp_sendto(&udp_conn, state_message, strlen(state_message), &server_ipaddr);
        
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}