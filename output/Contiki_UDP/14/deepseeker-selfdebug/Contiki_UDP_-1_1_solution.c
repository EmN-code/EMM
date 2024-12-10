
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

#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"

static struct simple_udp_connection udp_conn;
static struct etimer et;
static char device_state[10];

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
    // Optional: Handle incoming UDP packets if needed
}

PROCESS_THREAD(udp_client_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

    // Set initial device state
    strcpy(device_state, DEVICE_STATE_WORK);

    while(1) {
        // Send device state every 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send device state to UDP server
        simple_udp_sendto(&udp_conn, device_state, strlen(device_state), uip_ipaddr(UDP_SERVER_IP));

        // Toggle device state
        if(strcmp(device_state, DEVICE_STATE_WORK) == 0) {
            strcpy(device_state, DEVICE_STATE_NOT_WORK);
        } else {
            strcpy(device_state, DEVICE_STATE_WORK);
        }
    }

    PROCESS_END();
}
