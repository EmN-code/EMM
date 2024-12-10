#include "contiki.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_IP "47.102.103.1"

static struct uip_udp_conn *client_conn;
static char *state_message = "work or not work";

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    client_conn = uip_udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT));
    if (client_conn == NULL) {
        PROCESS_EXIT();
    }
    uip_ipaddr_t server_ipaddr;
    uip_ipaddr(&server_ipaddr,
               47,102,103,1);
    uip_udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        uip_udp_packet_sendto(client_conn, state_message, strlen(state_message),
                              &server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));
        etimer_reset(&timer);
    }

    PROCESS_END();
}