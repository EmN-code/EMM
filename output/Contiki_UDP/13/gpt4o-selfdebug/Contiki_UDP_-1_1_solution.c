#include <stdio.h>
#include "sys/etimer.h"
#include "net/netstack.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/simple-udp.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND) // Sending interval of 5 seconds

static struct simple_udp_connection udp_conn;

PROCESS(udp_sender_process, "UDP Sender Process");
AUTOSTART_PROCESSES(&udp_sender_process);

PROCESS_THREAD(udp_sender_process, ev, data) {
    static struct etimer periodic_timer;
    uip_ipaddr_t server_ipaddr;

    PROCESS_BEGIN();

    // Configure destination IPv4 server address
    uip_ip6addr(&server_ipaddr, 0x002f,0x0a66,0x4001,0x0000,0x0000,0x0000,0x0000,0x0000);

    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    // Set periodic timer for sending UDP packets
    etimer_set(&periodic_timer, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        // Send device state to the server
        simple_udp_sendto(&udp_conn, DEVICE_STATE, sizeof(DEVICE_STATE), &server_ipaddr);

        // Reset the timer
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}