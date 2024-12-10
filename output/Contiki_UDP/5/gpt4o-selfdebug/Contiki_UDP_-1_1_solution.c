#include "simple-udp.h"
#include "contiki-net.h"
#include "contiki.h"
#include <stdio.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer timer;
    uip_ipaddr_t server_ipaddr;
    char *msg = "work"; // Message to send

    PROCESS_BEGIN();

    // Convert the server IP address from text to binary format
    if (!uiplib_ipaddrconv(SERVER_IP, &server_ipaddr)) {
        printf("Failed to convert IP address\n");
        PROCESS_EXIT();
    }

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    // Set the timer to send a message every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Send UDP message
        simple_udp_sendto(&udp_conn, msg, strlen(msg), &server_ipaddr);
        printf("Message sent: %s\n", msg);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}