
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"

#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"

static struct uip_udp_conn *conn;
static struct etimer timer;

void send_udp_message(const char *message) {
    uip_ipaddr_t addr;
    uip_ipaddr(&addr, 47, 102, 103, 1);
    uip_udp_packet_send(conn, message, strlen(message));
}

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the UDP connection
    conn = uip_udp_new(&addr, UIP_HTONS(UDP_SERVER_PORT));
    if(conn == NULL) {
        printf("Error: could not create UDP connection\n");
        PROCESS_EXIT();
    }

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Send the device state
        send_udp_message(DEVICE_STATE);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
