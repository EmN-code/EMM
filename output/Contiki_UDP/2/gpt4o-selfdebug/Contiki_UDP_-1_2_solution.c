#include "contiki.h"
#include "net/ip/uip.h"
#include "simple-udp.h"

#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 1234
#define PERIOD 5

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_connection;

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer periodic_timer;
    uip_ipaddr_t server_ipaddr;
    char send_buf[20];

    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_connection, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    // Convert the IP address
    uiplib_ipaddrconv(UDP_SERVER_IP, &server_ipaddr);

    // Set a timer for periodic message sending
    etimer_set(&periodic_timer, CLOCK_SECOND * PERIOD);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&periodic_timer)) {
            // Prepare the message
            snprintf(send_buf, sizeof(send_buf), "Device is working");

            // Send the message
            simple_udp_sendto(&udp_connection, send_buf, strlen(send_buf) + 1, &server_ipaddr);

            // Reset the timer
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}

int main() {
    process_start(&udp_client_process, NULL);
    return 0;
}