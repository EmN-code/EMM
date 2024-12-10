
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"
#include "net/ip/simple-udp.h"
#include "sys/clock.h"

#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"

static struct simple_udp_connection udp_conn;

PROCESS(device_state_sender, "Device State Sender");
AUTOSTART_PROCESSES(&device_state_sender);

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t data_len)
{
    // Optional: Handle incoming UDP packets if needed
}

PROCESS_THREAD(device_state_sender, ev, data)
{
    static struct etimer et;
    uip_ipaddr_t server_ip;

    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, udp_rx_callback);

    // Set the server IP address
    uip_ipaddr_copy(&server_ip, uip_ip6addr(UDP_SERVER_IP));

    // Set the timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state message
        simple_udp_sendto(&udp_conn, DEVICE_STATE_MESSAGE, strlen(DEVICE_STATE_MESSAGE), &server_ip, UDP_SERVER_PORT);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}
