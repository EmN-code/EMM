
#include <stdio.h>
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/simple-udp.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define UDP_CLIENT_PORT 12345
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_IP "47.102.103.1"

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen)
{
    printf("Received response from server\n");
}

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer et;
    static char state[] = "work";
    uip_ipaddr_t server_ip;

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

    uip_ipaddr_from_str(&server_ip, UDP_SERVER_IP);

    etimer_set(&et, 5 * CLOCK_SECOND);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        simple_udp_sendto(&udp_conn, state, sizeof(state), &server_ip);

        etimer_reset(&et);
    }

    PROCESS_END();
}
