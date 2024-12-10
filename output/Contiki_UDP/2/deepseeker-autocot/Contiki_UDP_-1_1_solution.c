
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345
#define DEVICE_STATE "work"  // or "not work"

static struct simple_udp_connection udp_conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void udp_rx_callback(struct simple_udp_connection *c,
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
    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

    // Set up the periodic timer
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Prepare the message
        char message[20];
        snprintf(message, sizeof(message), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        uip_ipaddr_t server_ip;
        uip_ip6addr(&server_ip, 0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);  // Example IP, replace with actual server IP
        simple_udp_sendto(&udp_conn, message, strlen(message), &server_ip);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}
