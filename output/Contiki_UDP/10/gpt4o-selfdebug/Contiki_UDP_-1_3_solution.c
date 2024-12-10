#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip.h"

#include <stdio.h>
#include "contiki.h"
#include "net/ipv6/simple-udp.h"

#define UDP_PORT 10001
#define UDP_SERVER_IP "47.102.103.1"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer periodic_timer;
    static char *message = "work";

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

    etimer_set(&periodic_timer, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        uip_ipaddr_t server_ipaddr;
        uiplib_ipaddrconv(UDP_SERVER_IP, &server_ipaddr);

        printf("Sending message: %s\n", message);
        simple_udp_sendto(&udp_conn, message, strlen(message), &server_ipaddr);

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}