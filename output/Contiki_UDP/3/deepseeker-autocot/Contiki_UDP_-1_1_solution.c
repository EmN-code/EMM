
#include "contiki.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "dev/leds.h"
#include "simple-udp.h"

#define UDP_PORT 20001
#define SERVER_IP "47.102.103.1"
#define DEVICE_STATE "work"

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
    static char state_msg[] = DEVICE_STATE;

    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);

    // Set the timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state message
        simple_udp_sendto(&udp_conn, state_msg, sizeof(state_msg), uip_ipaddr(SERVER_IP));

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}
