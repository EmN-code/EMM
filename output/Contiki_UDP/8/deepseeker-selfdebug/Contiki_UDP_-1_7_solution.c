
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work" // or "not work"

static struct uip_udp_conn *conn;
static struct etimer et;

void udp_send_state(const char *state) {
    uip_ipaddr_t addr;
    uip_ipaddr(&addr, 47, 102, 103, 1);
    uip_udp_packet_send(conn, state, strlen(state));
}

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the UDP connection
    conn = uip_udp_new(&addr, UIP_HTONS(UDP_SERVER_PORT));
    if (conn == NULL) {
        printf("Failed to create UDP connection\n");
        PROCESS_EXIT();
    }

    // Set the timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Reset the timer
        etimer_reset(&et);

        // Send the device state
        udp_send_state(DEVICE_STATE);
    }

    PROCESS_END();
}

int main() {
    // Start the Contiki system
    process_start(&udp_client_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
