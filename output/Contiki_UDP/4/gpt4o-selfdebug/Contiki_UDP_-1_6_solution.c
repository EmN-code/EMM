#include "uip.h"
#include "uip-udp-packet.h"
#include <contiki.h>
#include <dev/leds.h>
#include <stdio.h>

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *conn;

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer timer;
    uip_ipaddr_t server_ipaddr;

    PROCESS_BEGIN();

    uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);

    conn = uip_udp_new(&server_ipaddr, UIP_HTONS(SERVER_PORT));
    if(conn == NULL) {
        printf("No UDP connection available, exiting the process\n");
        PROCESS_EXIT();
    }

    etimer_set(&timer, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);

        const char *state = "work";
        uip_udp_packet_sendto(conn, state, strlen(state),
                              &server_ipaddr, UIP_HTONS(SERVER_PORT));
        printf("Sent UDP packet to %s:%d\n", SERVER_IP, SERVER_PORT);

        leds_toggle(LEDS_RED);
    }

    PROCESS_END();
}