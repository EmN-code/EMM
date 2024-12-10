
#include "contiki.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_udp_message(const char *message) {
    uip_ipaddr_t server_ip;
    uiplib_ipaddrconv(UDP_SERVER_IP, &server_ip);

    uip_udp_packet_sendto(conn, message, strlen(message), &server_ip, UIP_HTONS(UDP_SERVER_PORT));
}

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    // Create a UDP connection
    conn = udp_new(NULL, UIP_HTONS(0), NULL);
    udp_bind(conn, UIP_HTONS(0));

    // Start the periodic timer
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state
        send_udp_message(DEVICE_STATE);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}
