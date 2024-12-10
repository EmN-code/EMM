
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include <string.h>
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work" // or "not work"

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_udp_message(const char *message) {
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, 47, 102, 103, 1);
    uip_udp_packet_send(conn, message, strlen(message));
}

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize UDP connection
    conn = uip_udp_new(&server_ip, UIP_HTONS(UDP_SERVER_PORT));
    if(conn == NULL) {
        printf("Error: could not create UDP connection\n");
        PROCESS_EXIT();
    }

    // Set up periodic timer
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send device state
        send_udp_message(DEVICE_STATE);

        // Reset timer
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    // Start the Contiki system
    process_start(&udp_client_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
