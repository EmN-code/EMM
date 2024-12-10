#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include <stdio.h>

static struct simple_udp_connection udp_conn;
uip_ipaddr_t server_ipaddr;

void udp_rx_callback(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr,
                     uint16_t sender_port, const uip_ipaddr_t *receiver_addr,
                     uint16_t receiver_port, const uint8_t *data, uint16_t datalen) {
    // Handle received data
    printf("Received response from server\n");
}

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer periodic_timer;
    PROCESS_BEGIN();

    // Set the server IP address
    uip_ip6addr(&server_ipaddr, 47, 102, 103, 1);

    // Register a UDP connection
    simple_udp_register(&udp_conn, 1234, NULL, 20001, udp_rx_callback);

    // Set the timer to send packets periodically every 5 seconds
    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        // Send the message "work" to server
        uip_udp_packet_sendto(&udp_conn, "work", 4, &server_ipaddr, UIP_HTONS(20001));

        // Restart the timer
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}