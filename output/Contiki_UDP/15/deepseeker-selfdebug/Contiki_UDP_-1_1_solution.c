
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(udp_sender_process, "UDP Sender Process");
AUTOSTART_PROCESSES(&udp_sender_process);

static void udp_send_state(void) {
    static char buffer[32];
    strncpy(buffer, DEVICE_STATE_MESSAGE, sizeof(buffer));
    simple_udp_send(&udp_conn, buffer, strlen(buffer));
}

PROCESS_THREAD(udp_sender_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_SERVER_PORT, NULL);

    // Set up the periodic timer
    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state
        udp_send_state();

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    process_init();
    process_start(&udp_sender_process, NULL);
    autostart_start(autostart_processes);

    // Main loop
    while(1) {
        process_run();
    }

    return 0;
}
