
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345

#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static struct etimer et;

static char device_state[] = "work"; // or "not work"

void udp_send_state(void) {
    simple_udp_sendto(&udp_conn, device_state, sizeof(device_state), (struct uip_ipaddr *)UDP_SERVER_IP, UDP_SERVER_PORT);
}

PROCESS(device_state_sender, "Device State Sender");
AUTOSTART_PROCESSES(&device_state_sender);

PROCESS_THREAD(device_state_sender, ev, data) {
    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    // Set the timer for the first event
    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state
        udp_send_state();

        // Reset the timer for the next event
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    // Start the Contiki system
    process_start(&device_state_sender, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
