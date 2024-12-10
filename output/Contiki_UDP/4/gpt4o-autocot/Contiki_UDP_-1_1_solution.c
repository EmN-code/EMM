#include <stdio.h>
#include <stdint.h>
#include "net/ip/uip.h"
#include "contiki.h"
#include "simple-udp.h"

#define UDP_SERVER_ADDRESS "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(nrf52840_udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&nrf52840_udp_client_process);

static struct simple_udp_connection udp_conn;
static char* device_state = "work";

PROCESS_THREAD(nrf52840_udp_client_process, ev, data) {
    static struct etimer periodic_timer;

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    etimer_set(&periodic_timer, SEND_INTERVAL);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        // Toggle device state
        if(strcmp(device_state, "work") == 0) {
            device_state = "not work";
        } else {
            device_state = "work";
        }

        // Send the device state to the server
        simple_udp_send(&udp_conn, device_state, strlen(device_state) + 1);
        printf("Sent device state: %s\n", device_state);

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}

int main() {
    process_start(&nrf52840_udp_client_process, NULL);
    while(1) {
        // Let Contiki manage the process scheduling
    }
}