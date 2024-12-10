
#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "sys/etimer.h"
#include "dev/button-hal.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *udp_conn;
static struct etimer et;
static char device_state[10];

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_udp_message(const char *message) {
    uip_ipaddr_t addr;
    uip_ipaddr(&addr, 47, 102, 103, 1);
    uip_udp_packet_send(udp_conn, message, strlen(message));
}

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the UDP connection
    udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    udp_bind(udp_conn, UIP_HTONS(0));

    // Set the device state to "work" initially
    strcpy(device_state, DEVICE_STATE_WORK);

    // Set the periodic timer
    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state to the UDP server
        send_udp_message(device_state);

        // Toggle the device state
        if (strcmp(device_state, DEVICE_STATE_WORK) == 0) {
            strcpy(device_state, DEVICE_STATE_NOT_WORK);
        } else {
            strcpy(device_state, DEVICE_STATE_WORK);
        }

        // Reset the timer for the next interval
        etimer_reset(&et);
    }

    PROCESS_END();
}
