
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
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static struct etimer et;

static char device_state[] = "work"; // or "not work"

void udp_send_state(void) {
    simple_udp_sendto(&udp_conn, device_state, strlen(device_state), (struct uip_ipaddr *)UDP_SERVER_IP, UDP_SERVER_PORT);
    printf("Sent device state: %s\n", device_state);
}

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    // Set the timer for the first send
    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state
        udp_send_state();

        // Reset the timer for the next send
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    process_init();
    process_start(&udp_client_process, NULL);
    autostart_start(autostart_processes);

    // Main loop
    while(1) {
        process_run();
    }

    return 0;
}
