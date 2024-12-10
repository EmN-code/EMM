
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(device_state_sender, "Device State Sender");
AUTOSTART_PROCESSES(&device_state_sender);

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen)
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
    uip_ipaddr_from_str(&server_ip, UDP_SERVER_IP);

    // Start the periodic timer
    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state message
        simple_udp_sendto(&udp_conn, DEVICE_STATE_MESSAGE, sizeof(DEVICE_STATE_MESSAGE) - 1, &server_ip);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}
