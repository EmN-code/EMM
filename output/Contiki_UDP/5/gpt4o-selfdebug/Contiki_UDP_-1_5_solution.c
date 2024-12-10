#include "contiki.h"
#include "net/socket.h"
#include <string.h>

PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer timer;
    static struct udp_socket udp_conn;
    static uip_ipaddr_t dest_ipaddr;
    char *msg = "work";

    PROCESS_BEGIN();

    uip_ip6addr(&dest_ipaddr, 0x2f7d, 0x6605, 0x400, 0x757b, 0, 0, 0, 1);

    udp_socket_register(&udp_conn, NULL, NULL);
    udp_socket_connect(&udp_conn, &dest_ipaddr, 20001);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        if(strcmp(msg, "work") == 0) {
            udp_socket_sendto(&udp_conn, msg, strlen(msg), &dest_ipaddr, 20001);
        }
        etimer_reset(&timer);
    }

    PROCESS_END();
}