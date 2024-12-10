#include <string.h>
#include <stdio.h>
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"

#define UDP_PORT 20001
#define SEND_INTERVAL (CLOCK_SECOND * 5)

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen) {
    printf("Received response from server: %s\n", data);
}

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer periodic_timer;
    static char msg[] = "work";
    uip_ipaddr_t server_ipaddr;

    PROCESS_BEGIN();

    // Convert server IP address from text to binary form
    uip_ip6addr(&server_ipaddr, 0x47, 0x102, 0x103, 0x1, 0, 0, 0, 0x1);

    // Setup the UDP connection
    simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);

    etimer_set(&periodic_timer, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        printf("Sending message to %d\n", server_ipaddr.u8[15]);

        // Send packet to the server IP address
        simple_udp_sendto(&udp_conn, msg, sizeof(msg), &server_ipaddr);

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}